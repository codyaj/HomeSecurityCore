// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#ifndef MESSAGEUTILS_H
#define MESSAGEUTILS_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "cryptoUtils.h"
#include "alarmControl.h"
#include "heartbeat.h"

#define FIXED_MESSAGE_SIZE 40

#define INCOMMING_HEARTBEAT 0x00
#define INCOMMING_ENABLE_ALARM 0x01
#define INCOMMING_DISABLE_ALARM 0x02
#define INCOMMING_STATUS_REQUEST 0x03
#define INCOMMING_FIRMWARE_UPDATE 0x04
#define INCOMMING_TESTING_SEQUENCE 0x05
#define OUTGOING_HEARTBEAT 0x00
#define OUTGOING_JAMMING_DETECTED 0x01
#define OUTGOING_STATUS_REPLY 0x03
#define OUTGOING_TAMPER_ALERT 0x04
#define OUTGOING_LOW_BATTERY 0x05

/*
 * @brief Adds a nonce to the current list of used nonces
 *
 * @param[nonce8] An array of 4 bytes which are the nonce (input)
 * @warning nonce8 must be an array of size 4
 */
void addNonce(uint8_t nonce8[]);

/*
 * @brief Checks the input nonce against all currently stored nonces
 *
 * @param[nonce8] The nonce you wish to check (input)
 * @return True if the nonce is currently stored. False if the nonce is not currently stored
 * @warning nonce8 must be an array of size 4
 */
bool checkNonce(uint8_t nonce8[]);

/*
 * @brief Generates a random 4 byte block to be used for the nonce
 *
 * @param[nonce] An array to return the generated nonce too (output)
 * @warning nonce must be an array of size 4
 */
void generateNonce(uint8_t* nonce);

/*
 * @brief Receives and handles incomming messages
 *
 * @note This should be run as often as possible to avoid delays from sending to reading
 */
void recvMessage();

/*
 * @brief Sends a message with no content to the server
 *
 * @param[messageType] a byte containing the message type being sent to the server (input)
 * @return True if the message successfully sent. False if the message did not send.
 */
bool sendMessage(uint8_t messageType);

/*
 * @brief Sends a message with content to the server
 *
 * @param[messageType] a byte containing the message type being sent to the server (input)
 * @param[data] A pointer to an array that contains the data being sent (input)
 * @param[dataSize] The size of the data being sent in bytes (input)
 * @return True if the message successfully sent. False if the message did not send.
 * @warning dataSize must be equal to the size of the data in bytes.
 */
bool sendMessage(uint8_t messageType, uint8_t* data, uint8_t dataSize);

#endif
