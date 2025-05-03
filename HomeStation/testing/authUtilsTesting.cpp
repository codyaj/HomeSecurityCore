// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#include "../include/authUtils.h"
#include <iostream>

// Tests nonce generation functionality
void testNonce() {
    std::array<uint8_t, 4> Nonce;
    generateNonce(Nonce);
    std::cout << "Nonce: ";
    for (uint8_t byte : Nonce) {
        std::cout << std::hex << static_cast<int>(byte) << std::dec;
    }
    std::cout << "\n";
}

/* Tests HMAC generation with known values
 * Verified externally using https://cryptii.com/pipes/hmac
 * Date of verification: 16/12/24
 */
void testHMAC() {
    std::array<uint8_t, 16> key = {
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41
    };

    std::vector<uint8_t> message = {
        0x41, 0x42, 0x43, 0x44, 0x45,
        0x46, 0x47, 0x48, 0x49, 0x50
    };

    std::array<uint8_t, 32> outputHMAC;

    generateHMAC(outputHMAC.data(), key, message);

    std::cout << "Message: ";
    for (uint8_t byte : message) {
        std::cout << std::hex << static_cast<int>(byte) << std::dec;
    }

    std::cout << "\nKey: ";
    for (uint8_t byte : key) {
        std::cout << std::hex << static_cast<int>(byte) << std::dec;
    }
    
    std::cout << "\nHMAC: ";
    for (uint8_t byte : outputHMAC) {
        std::cout << std::hex << static_cast<int>(byte) << std::dec;
    }
    std::cout << "\n";
}

int main() {
    // Uncomment to test nonce generation
    // std::cout << "Testing Nonce\n";
    // testNonce();

    std::cout << "Testing HMAC\n";
    testHMAC();

    return 0;
}
