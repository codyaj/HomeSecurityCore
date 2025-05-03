// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#include "messageUtils.h"

// Helper function. Turns an uint8_t array of size 4 to a uint32_t
uint32_t convertTo32Bit(uint8_t arr[]) {
  uint32_t result = 0;
  result |= (static_cast<uint32_t>(arr[0]) << 24);
  result |= (static_cast<uint32_t>(arr[1]) << 16);
  result |= (static_cast<uint32_t>(arr[2]) << 8);
  result |= (static_cast<uint32_t>(arr[3]));
  return result;
}

uint32_t prevNonce[MAX_NONCE] = {0};
// ? Circ buffer would be more efficient
void addNonce(uint8_t nonce8[]) {
  uint32_t nonce = convertTo32Bit(nonce8);
  for (int i = MAX_NONCE - 1; i > 0; i--) {
    prevNonce[i] = prevNonce[i - 1];
  }
  prevNonce[0] = nonce;
}

bool checkNonce(uint8_t nonce8[]) {
  uint32_t nonce = convertTo32Bit(nonce8);
  for (auto pastNonce : prevNonce) {
    if (pastNonce == nonce) {
      return true;
    }
  }
  return false;
}

void generateNonce(uint8_t* nonce) {
  RNG rng;
  rng.fill(nonce, 4);
}


extern WiFiClient client;


void recvMessage() {
  char buff[8];

  // Read Header
  if (client.read(buff, 8) != 8) {
    Serial.println("Failed to read the full header");
    return;
  }
    
  uint8_t header[] = {buff[0], buff[1]};
  uint8_t msgType = buff[2];
  uint8_t nonce[] = {buff[3], buff[4], buff[5], buff[6]};
  uint8_t payloadLen = buff[7];
  
  // Verify the header
  if (header[0] != 0xA5 || header[1] != 0x5A) {
    Serial.println("[DEBUG] Message header check failed");
    return;
  }

  // Verify Nonce
  if (checkNonce(nonce)) {
    Serial.println("[DEBUG] Nonce check failed");
    return;
  }
  addNonce(nonce);

  // Read Payload
  uint8_t* payload = nullptr;
  if (payloadLen != 0) {
    payload = (uint8_t*)malloc(payloadLen);
    if (!payload) {
      Serial.println("[DEBUG] Failed to allocate memory for payload");
      return;
    }

    if (client.read(payload, payloadLen) != payloadLen) {
      Serial.println("[DEBUG] Failed to read the full payload");
      free(payload);
      return;
    }
  }

  // Read HMAC
  uint8_t HMAC[32];
  if (client.read(HMAC, 32) != 32) {
    Serial.println("[DEBUG] Failed to read the full HMAC");
    free(payload);
    return;
  }

  // Reconstruct message for HMAC
  int messageLen = 8 + payloadLen; // Header + payload
  uint8_t* message = (uint8_t*)malloc(messageLen);
  if (!message) {
    Serial.println("[DEBUG] Failed to allocate memory for message");
    free(payload);
    return;
  }

  memcpy(message, buff, 8); // Copy header
  if (payloadLen != 0) {
    memcpy(message + 8, payload, payloadLen); // Copy payload
  }

  // Verfiy HMAC
  if (!compareHMAC(HMAC, message, messageLen)) {
    Serial.println("[DEBUG] HMAC did not match");
    free(payload);
    free(message);
    return;
  }

  // Handle Payload
  switch (msgType) {
  case INCOMMING_HEARTBEAT:
    Serial.println("HeartBeat");
    receivedHeartbeat();
    break;
  case INCOMMING_ENABLE_ALARM:
    Serial.println("Enable Alarm");
    enableAlarm(incommingAlert);
    break;
  case INCOMMING_DISABLE_ALARM:
    Serial.println("Disable Alarm");
    disableAlarm(incommingAlert);
    break;
  case INCOMMING_STATUS_REQUEST:
    Serial.println("Status Request");
    break;
  case INCOMMING_FIRMWARE_UPDATE:
    Serial.println("Firmware Update");
    break;
  case INCOMMING_TESTING_SEQUENCE:
    Serial.println("Testing Sequence");
    break;
  default:
    Serial.print("[DEBUG] Unknown message type: ");
    Serial.println(msgType, HEX);
  }
  Serial.print("Message Successfully Received: 0x");
  Serial.println(msgType, HEX); 

  // ! TODO
  if (payloadLen != 0) {

  }

  // Free Payload & Message
  free(payload);
  free(message);
}

bool sendMessage(uint8_t messageType) {
  // construct message
  uint8_t payload[FIXED_MESSAGE_SIZE] = {0xA5, 0x5A, messageType, 0, 0, 0, 0, 0};

  // Add nonce
  generateNonce(payload + 3);

  // Generate and add HMAC
  generateHMAC(payload + 8, payload, 8);

  // ? Debug
  Serial.print("[DEBUG] Payload: ");
  for (auto x : payload) {
    Serial.print(x, HEX);
    Serial.print(" ");
  }
  Serial.println();
  // ? Debug

  // send message
  size_t sentBytes = client.write(payload, sizeof(payload));

  // Check if the return of client.print(data) is equal to the bytes intended to send
  return sentBytes == FIXED_MESSAGE_SIZE;
}

// ! TODO
bool sendMessage(uint8_t messageType, uint8_t* data, uint8_t dataSize) {
  // Ensure adherence to arrays divisible by the block size
  if (dataSize % BLOCK_SIZE != 0) {
    Serial.println("[DEBUG] data size must be divisible by 16. Make the array larger to fix this.");
    return false;
  }

  // construct message
  uint8_t* payload = (uint8_t*)malloc(FIXED_MESSAGE_SIZE + dataSize);
  if (!payload) {
    Serial.println("[DEBUG] Unable to allocate payload!");
    return false;
  }

  payload[0] = 0xA5;
  payload[1] = 0x5A;
  payload[2] = messageType;
  generateNonce(payload + 3);
  payload[7] = dataSize / BLOCK_SIZE;

  // Encrypt message
  //encrypt(data, payload + 8, dataSize / BLOCK_SIZE);

  // ? Debug
  Serial.print("[DEBUG] Payload: ");
  for (int i = 0; i < FIXED_MESSAGE_SIZE + dataSize; i++) {
    Serial.print(payload[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  // ? Debug

  // send message with client.print(data)


  free(payload);

  // Check if the return of client.print(data) is equal to the bytes intended to send
  
  return true;
  
}
