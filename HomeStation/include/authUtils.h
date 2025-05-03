// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#ifndef AUTHUTILS_H
#define AUTHUTILS_H

#include <array>          // Arrays
#include <vector>         // Vectors
#include <cstdint>        // uint8_t
#include <random>         // Nonce
#include <openssl/hmac.h> // HMAC
#include <stdexcept>      // Error Handling

#define BLOCK_SIZE 64
#define SHA256_SIZE 32
#define maxNonce 20

class NonceStorage {
private:
    int curr; // Keeps track of current circular buffer location
    std::array<uint32_t, maxNonce> nonceCollection;

    // Helper function to convert 4 byte array to 32 bit integer
    uint32_t convertTo32Bit(std::array<uint8_t, 4> arr);
public:
    // Function to check if a nonce already exists in the buffer
    bool checkNonce(std::array<uint8_t, 4> nonceArr);

    // Function to add a new nonce to the circular buffer
    bool addNonce(std::array<uint8_t, 4> nonceArr) ;
};

/*
 *@brief Generates a Nonce
 *
 *@param nonce - An array of 4 bytes that will be used to store the Nonce
 */
void generateNonce(std::array<uint8_t, 4>& nonce);

/*
 *@brief Generates a HMAC with the input and key
 *
 *@param outputHMAC - Must be a pointer to an array of 32 bytes. Stores the final HMAC
 *       key        - A 16 byte key
 *       message    - vector containing the Header, Nonce, and Encrypted payload
 *
 *@throws std::runtime_error if HMAC generation fails
 */
void generateHMAC(uint8_t* outputHMAC, const std::array<uint8_t, 16>& key, const std::vector<uint8_t>& message);


#endif // AUTHUTILS_H
