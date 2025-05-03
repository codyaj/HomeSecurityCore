// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#include "../include/device.h"

#define INCOMMING_HEARTBEAT 0x00
#define OUTGOING_HEARTBEAT 0x00


bool Device::compareHMAC(std::array<uint8_t, 32> HMAC, std::vector<uint8_t> message) {
    std::array<uint8_t, 32> newHMAC;
    generateHMAC(newHMAC.data(), iv, message);

    return HMAC == newHMAC;
}

void Device::encrypt(std::vector<uint8_t>& output,const std::array<uint8_t, 32> key, const std::array<uint8_t, 16> iv, const std::vector<uint8_t> message) {
    EVP_CIPHER_CTX *ctx;
    int len; // For handling lengths during encryption
    int ciphertext_len; // Total length of the cipher text

    // Create and init the content
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        throw std::runtime_error("Failed to create encryption context");
    }
        
    // Init encytpion operation
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize encryption");
    }

    output.resize(message.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));

    // Provide the message to be encrypted and obtain encrypted output
    if (1 != EVP_EncryptUpdate(ctx, output.data(), &len, message.data(), message.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed during encryption update");
    }
    ciphertext_len = len;

    // Finalize encryption to ensure all blocks are processed
    if (1 != EVP_EncryptFinal_ex(ctx, output.data() + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed during encryption finalization");
    }
    ciphertext_len += len;

    // Resize output to the actual size of the ciphertext
    output.resize(ciphertext_len);

    // Clean up the content
    EVP_CIPHER_CTX_free(ctx);
}

std::vector<uint8_t> Device::decrypt(const std::array<uint8_t, 32>& key, const std::array<uint8_t, 16>& iv, const std::vector<uint8_t>& ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len; // For handling lengths during decryption
    int plaintext_len; // Total length of the plaintext

    // Create and initialize the context
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        throw std::runtime_error("Failed to create encryption context");
    }

    // Initialize decryption operation with AES-256-CBC
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize decryption");
    }

    std::vector<uint8_t> plaintext(ciphertext.size());

    // Decrypt the message and store the result in `output`
    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed during decryption update");
    }
    plaintext_len = len;

    // Finalize decryption to ensure all blocks are processed
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed during decryption finalization");
    }
    plaintext_len += len; // Add the final block length

    // Clean up the context
    EVP_CIPHER_CTX_free(ctx);

    // Resize vector to actual size
    plaintext.resize(plaintext_len);

    return plaintext; // Return the length of the decrypted plaintext
}

bool Device::shareKeys() {// ! Needs to be implemented
        // Agree on an elliptic curve and a base point

        // Generate a large random number for private key

        // Compute the public key
        // P_a = d_a * G where G is the base point and d_a is the private key

        // Exchange the public key

        // Generate the shared secret
        // S_a = d_a * P_b
    Device::key = {
        0x1C, 0x3E, 0x4B, 0xAF, 0x13, 0x4A, 0x89, 0xC3,
        0xF3, 0x87, 0x4F, 0xBC, 0xD7, 0x11, 0x22, 0x33,
        0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB,
        0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33
    };
    Device::iv = {0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41};
    return true;
}

void Device::handleIncommingHeartbeat(std::vector<uint8_t> heartbeatMessage) {
    if (heartbeatMessage == lastHeartbeatMessage) { // If the incomming heartbeat matches the latest outgoing heartbeat
        lastestHeartbeatTime = std::time(0);
    }
}

Device::Device(int Socket_fd) : socket_fd(Socket_fd) {
    shareKeys();
}

void Device::sendHeartbeat() {
    std::cout << "[DEBUG] Sending a heartbeat!\n";

    // Create unique heartbeat message and convert it to a vector
    std::array<uint8_t, 4> messageArr;
    generateNonce(messageArr);
    std::vector<uint8_t> message;
    message.insert(message.end(), messageArr.begin(), messageArr.end());

    // Send message
    if (sendMessage(HEARTBEAT_MESSAGE_TYPE, message)) {
        // Record details of the message if successfully sent
        lastHeartbeatMessage = message;
    }
}

ssize_t Device::receiveOnSocket(uint8_t* buffer, ssize_t bufferSize) {
    ssize_t rec = 0;
    do {
        int result = recv(Device::socket_fd, buffer, bufferSize, 0);
        if (result == -1) {
            // Handle error
            std::cout << "Error :(\n";
            break;
        } else if (result == 0) {
            // Handle disconnect
            std::cout << "Disconnected :(\n";
            break;
        } else {
            rec += result;
        }
    } while (rec < bufferSize);
    return rec;
}

// * Fix undefined reference problem in the sendMessage function
constexpr int Device::retryDelayMs;
constexpr int Device::maxRetries;

bool Device::sendMessage(uint8_t messageType, std::vector<uint8_t> message) {
    std::vector<uint8_t> payload;

    // Add Message Header
    payload.push_back(0xA5);
    payload.push_back(0x5A);

    // Add message type
    payload.push_back(messageType);

    // Generate Nonce
    std::array<uint8_t, 4> nonce;
    do {
        generateNonce(nonce);
    } while (outgoingNonce.checkNonce(nonce)); // Ensure Nonce is not already in the current buffer
    outgoingNonce.addNonce(nonce);

    // Add nonce to the payload
    payload.insert(payload.end(), nonce.begin(), nonce.end());

    // Add payload length to message
    payload.push_back((message.size() + 15) / 16);

    // Add payload (if applicable)
    if (!message.empty()) {
        // Encrypt Message
        std::vector<uint8_t> encryptedOutput;
        try {
            encrypt(encryptedOutput, key, iv, message);
        } catch (const std::runtime_error &e) {
            std::cerr << e.what();
            return false;
        }
        payload.insert(payload.end(), encryptedOutput.begin(), encryptedOutput.end());
    }

    // Calculate HMAC
    std::array<uint8_t, 32> outputHMAC;
    generateHMAC(outputHMAC.data(), iv, payload);
    std::cout << "IV: ";
    for (auto x : iv) {
        std::cout << std::hex<< static_cast<int>(x) << " ";
    }
    std::cout << std::endl;
    std::cout << "Data: ";
    for (auto x : payload) {
        std::cout << std::hex<< static_cast<int>(x) << " ";
    }
    std::cout << std::endl;
    payload.insert(payload.end(), outputHMAC.begin(), outputHMAC.end());

    // Send the payload
    for (int attempt = 0; attempt <= Device::maxRetries; attempt++) {
        ssize_t bytesSent = send(socket_fd, payload.data(), payload.size(), 0);
            
        if (bytesSent == static_cast<ssize_t>(payload.size())) {
            return true; // Successfully sent
        }

        // If send failed wait before trying again
        if (attempt < Device::maxRetries) {
            std::this_thread::sleep_for(std::chrono::milliseconds(Device::retryDelayMs));
        }
    }

    return false;
}

bool Device::isDataAvailable() {
    fd_set read_fds;
    struct timeval timeout;

    // Init the file descriptor set
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);

    // Set the timeout to zero for non-blocking check
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    // Check if there is data available to read
    int result = select(Device::socket_fd + 1, &read_fds, nullptr, nullptr, &timeout);
    if (result > 0 && FD_ISSET(socket_fd, &read_fds)) {
        return true; // data available
    } else if (result < 0) {
        perror("select() error");
    }

    return false; // No data
}

void Device::receiveMessage() {
    // Get packet Header & Nonce & Size
    std::array<uint8_t, 8> buff = {0};

    /*int bytesReceived = receiveOnSocket(buff.data(), 8);
    if (bytesReceived < 0) {
        perror("Receive failed");
        return;
    }*/

    receiveOnSocket(buff.data(), sizeof(buff));

    std::cout << "[DEBUG] Data from header: ";
    for (auto x : buff) {
        printf("%x ", x);
    }
    std::cout << std::endl;

    std::array<uint8_t, 2> header = {buff[0], buff[1]};
    uint8_t msgType = buff[2];
    std::array<uint8_t, 4> nonce = {buff[3], buff[4], buff[5], buff[6]};
    uint8_t payloadLen = buff[7];

    // Verify Header
    if (header[0] != 0xA5 || header[1] != 0x5A) {
        std::cout << "[DEBUG] Message header check failed\n";
        std::cout << "[DEBUG] 0x" << std::hex << header[0] << " " << header[1] << "\n";
        return;
    }

    // Check Nonce
    if (incommingNonce.checkNonce(nonce)) {
        std::cout << "[DEBUG] Nonce check failed\n";
        return;
    }

    // Read payload
    //! Add a check for payload size and change how this is calculated
    std::vector<uint8_t> payload;
    if (payloadLen != 0) {
        //payload = std::make_unique<uint8_t[]>(payloadLen);
        //! Calculate how many bytes to get: eg if payloadLen = 3, calculate 3 * 16
        //! receiveOnSocket those calulated bytes into payload
    }

    // Read HMAC
    std::array<uint8_t, 32> hmacBuff;
    receiveOnSocket(hmacBuff.data(), sizeof(hmacBuff));

    std::cout << "[DEBUG] Data from HMAC: ";
    for (auto x : hmacBuff) {
        printf("%x ", x);
    }
    std::cout << std::endl;

    // Compare HMACs
    std::vector<uint8_t> messageVec(buff.begin(), buff.end());
    bool hmacTest = compareHMAC(hmacBuff, messageVec);
    if (!hmacTest) {
        std::cout << "[DEBUG] HMACs are not the same!\n";
    }

    // Decrypt message


    // Send to appropriate function
    if (msgType == INCOMMING_HEARTBEAT) {
        sendHeartbeat();
    } else if (payloadLen == 0) {
        handleMessage(msgType);
    } else {
        handleMessage(msgType, payload);
    }
}

Device::~Device() {}
