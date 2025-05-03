// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#include "cryptoUtils.h"

// Temporary hardcoded values
uint8_t key[KEY_LENGTH] = {
    0x1C, 0x3E, 0x4B, 0xAF, 0x13, 0x4A, 0x89, 0xC3,
    0xF3, 0x87, 0x4F, 0xBC, 0xD7, 0x11, 0x22, 0x33,
    0x1C, 0x3E, 0x4B, 0xAF, 0x13, 0x4A, 0x89, 0xC3,
    0xF3, 0x87, 0x4F, 0xBC, 0xD7, 0x11, 0x22, 0x33
};
uint8_t iv[16] = {0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41}; // Used for IV and HMAC key

void encrypt(uint8_t* plainText, uint8_t* output, int length) {
  // Not implemented
}

void decrypt(uint8_t* enciphered, uint8_t* output, int length) {
  // Not implemented
}

void generateHMAC(uint8_t* outputHMAC, uint8_t* msg, unsigned int msgLen) {
  SHA256HMAC hmac(iv, 16);

  hmac.doUpdate(msg, msgLen);

  hmac.doFinal(outputHMAC);
}

bool compareHMAC(uint8_t *HMAC, uint8_t msg[], unsigned int msgLen) {
    // Generate HMAC
    uint8_t newHMAC[SHA256HMAC_SIZE];
    generateHMAC(newHMAC, msg, msgLen);

    // Compare and return result
    return memcmp(newHMAC, HMAC, SHA256HMAC_SIZE) == 0;
}
