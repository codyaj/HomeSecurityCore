// This file is part of the Home Security Lite release.
// For educational or reference use only. Not for commercial redistribution.
// © 2025 Cody Jackson. All rights reserved.

#include "../include/authUtils.h"

void generateNonce(std::array<uint8_t, 4>& nonce) {
    static std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<uint8_t> dist(0, 255);

    // Generate random nonce
    for (uint8_t& byte : nonce) {
        byte = dist(mt);
    }
}

void generateHMAC(uint8_t* outputHMAC, const std::array<uint8_t, 16>& key, const std::vector<uint8_t>& message) {
    unsigned int len = 32;

    if(!HMAC(EVP_sha256(), key.data(), key.size(), message.data(), message.size(), outputHMAC, &len)) {
        throw std::runtime_error("HMAC generation failed");
    }
}


uint32_t NonceStorage::convertTo32Bit(std::array<uint8_t, 4> arr) {
    uint32_t result = 0;
    result |= (static_cast<uint32_t>(arr[0]) << 24);
    result |= (static_cast<uint32_t>(arr[1]) << 16);
    result |= (static_cast<uint32_t>(arr[2]) << 8);
    result |= (static_cast<uint32_t>(arr[3]));
    return result;
}

bool NonceStorage::checkNonce(std::array<uint8_t, 4> nonceArr) {
    uint32_t nonce = convertTo32Bit(nonceArr);

    for (int i = 0; i < maxNonce; i++) {
        if (nonceCollection[i] == nonce) {
            return true;
        }
    }

    return false;
}

bool NonceStorage::addNonce(std::array<uint8_t, 4> nonceArr) {
    uint32_t nonce = convertTo32Bit(nonceArr);

    nonceCollection[curr] = nonce;
    curr = (curr + 1) % maxNonce;

    return true;
}
