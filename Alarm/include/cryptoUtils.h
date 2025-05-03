// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#ifndef CRYPTOUTILS_H
#define CRYPTOUTILS_H

#include <Crypto.h>
#include <Arduino.h>

#define BLOCK_SIZE 16
#define KEY_LENGTH 32
extern uint8_t iv[16]; // Used for IV and HMAC key

/*
 * @brief Encrypts input text with AES256
 *
 * @param[plainText] pointer to plainText bytes to be encrypted
 * @param[output] pointer to where the encrypted output should be put
 * @param[length] Length in bytes of the plainText
 */
void encrypt(uint8_t* plainText, uint8_t* output, int length);

/*
 * @brief Decrypts the text from AES256
 *
 * @param[enciphered] pointer to encrypted bytes
 * @param[output] pointer to where the plain text output should be put
 * @param[length] Length in bytes of the enciphered text
 */
void decrypt(uint8_t* enciphered, uint8_t* output, int length);

/*
 * @brief Generates a HMAC with the given message
 * 
 * @param[outputHMAC] Pointer to the outputHMAC. Must be 32 bytes in size. (output)
 * @param[msg] Pointer to the message you wish to hash. (input)
 * @param[msgLen] The size in bytes of the message. (input)
 * @warning outputHMAC MUST be atleast 32 bytes in size.
 */
void generateHMAC(uint8_t* outputHMAC, uint8_t* msg, unsigned int msgLen);

/*
 * @brief Compares a given HMAC with one generated using the information given.
 * 
 * @param[HMAC] Pointer to the HMAC given. (input)
 * @param[msg] Pointer to the message you wish to hash. (input)
 * @param[msgLen] The size in bytes of the message. (input)
 * @return True if the message is authentic. False if the message is not authentic.
 */
bool compareHMAC(uint8_t *HMAC, uint8_t msg[], unsigned int msgLen);

#endif
