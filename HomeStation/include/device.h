// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>
#include <array>
#include <vector>
#include <ctime>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include "../include/authUtils.h"
#include <sys/select.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <openssl/evp.h>

#define HEARTBEAT_MESSAGE_TYPE 0

class Device {
private:
    std::time_t lastestHeartbeatTime;          // Time of last heartbeat
    std::vector<uint8_t> lastHeartbeatMessage; // The message of the last heartbeat (nonce)

    std::array<uint8_t, 32> key;
    std::array<uint8_t, 16> iv;  // Acts as both the encryption IV and the HMAC key

    NonceStorage outgoingNonce;  // For outgoing message

    // Retry settings
    static constexpr int retryDelayMs = 100;
    static constexpr int maxRetries = 3;

    /*
     *@brief Encrypts the contents of a message.
     *
     *@param output  - Vector where the encrypted ciphertext will be stored
     *       key     - Key used for encryption (256-bit / 32 bytes)
     *       iv      - IV used for encryption (128-bit / 16 bytes)
     *       message - The plaintext message to be encrypted
     */
    void encrypt(std::vector<uint8_t>& output,const std::array<uint8_t, 32> key, const std::array<uint8_t, 16> iv, const std::vector<uint8_t> message);

    /*
     *@brief Performs a Diffie-Hellman key exchange to generate and share a key and IV.
     */
    bool shareKeys();

    /*
     *@brief Verifies incoming heartbeat messages to ensure they match the last sent heartbeat.
     *       Also updates the timestamp of the last successful heartbeat.
     *
     *@param heartbeatMessage - The unique content of the incoming heartbeat message
     */
    void handleIncommingHeartbeat(std::vector<uint8_t> heartbeatMessage);


protected:
    int socket_fd;

    NonceStorage incommingNonce; // For incomming message
    
    /*
     *@brief Decrypts the contents of a ciphertext message.
     *
     *@param key        - Key used for decryption (256-bit / 32 bytes)
     *       iv         - IV used for decryption (128-bit / 16 bytes)
     *       ciphertext - The encrypted message
     * 
     *@return A vector containing the decrypted message bytes
     */
    std::vector<uint8_t> decrypt(const std::array<uint8_t, 32>& key, const std::array<uint8_t, 16>& iv, const std::vector<uint8_t>& ciphertext);

    /*
     *@brief Validates an incoming HMAC by comparing it to a locally generated one.
     *
     *@param HMAC    - Incoming HMAC value
     *       message - Message data (excluding HMAC) used for verification
     * 
     *@return True if the HMAC matches; false otherwise
     */
    bool compareHMAC(std::array<uint8_t, 32> HMAC, std::vector<uint8_t> message);

    /*
     *@brief Receives a specific number of bytes from the socket stream.
     *
     *@param buffer     - Pointer to the buffer to write into
     *       bufferSize - Number of bytes to receive
     * 
     *@return Number of bytes actually received
     */
    ssize_t receiveOnSocket(uint8_t* buffer, ssize_t bufferSize);
    
    /*
     *@brief Handles incoming messages without associated data.
     *@note This function is called after receiveMessage, once security validation passes.
     *
     *@param msgType - A byte representing the message type
     */
    virtual void handleMessage(uint8_t msgType) = 0;

    /*
     *@brief Handles incoming messages with associated data.
     *@note This function is called after receiveMessage, once security validation passes.
     *
     *@param msgType - A byte representing the message type
     *       data    - A vector of bytes containing the message's data
     */
    virtual void handleMessage(uint8_t msgType, std::vector<uint8_t> data) = 0;

    /*
     *@brief Initializes the Device instance.
     *
     *@param Socket_fd - File descriptor for the device socket
     */
    Device(int Socket_fd);

    /*
     *@brief Sends a heartbeat message with a unique nonce to the connected device.
     */
    void sendHeartbeat();

    /*
     *@brief Sends a message to the connected device.
     *
     *@param messageType - A 3-bit value representing the type of message being sent
     *       message     - A vector containing the message's data payload
     * 
     *@return True if the message was sent successfully; false otherwise
     */
    bool sendMessage(uint8_t messageType, std::vector<uint8_t> message);

    /*
     *@brief Checks whether there is data ready to be read from the socket.
     *
     *@return True if data is available; false otherwise
     */
    bool isDataAvailable();

    /*
     *@brief Receives and verifies an incoming message, then dispatches it to the appropriate handler.
     *@note After successful validation, this function calls handleMessage.
     */
    void receiveMessage();

    virtual ~Device();
};

#endif
