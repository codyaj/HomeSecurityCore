// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#include "main.h"


WiFiClient client;


char ssid[32];
char pass[63];

void setup() {
  Serial.begin(9600);
  
  Serial.println();


  establishKeys();

  while (1) {
    Serial.print("/");
    delay(1000);
  }


  setupResetBtn(); // Reset BTN
  initEEPROM();    // EEPROM
  setupAlarmPin(); // Alarm
  setupLED();      // RGB Leds

  // If unable to get WiFi Credentials init setup
  if (!getWiFiCredentials(ssid, pass)) {
    Serial.println("[DEBUG] No WiFi Credentials");
    updateLED(setupMode);
    beginSetup();
  }

  // Get new creds
  getWiFiCredentials(ssid, pass);
  Serial.println("[DEBUG] Found WiFi Credentials: ");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Pass: ");
  Serial.println(pass);

  // Connect to wifi
  updateLED(waiting);
  WiFi.begin(ssid, pass);
  Serial.print("[DEBUG] Connecting..");
  unsigned long lastDebugUpdate = 0;
  while (WiFi.status() != WL_CONNECTED) {
    // ? DEBUG
    if (millis() >= lastDebugUpdate + 1000) {
      Serial.print(".");
      lastDebugUpdate = millis();
    }
    // ? DEBUG
    updateLED();
    checkResetBtn(); // Incase unable to connect and want to reinput the credentials
  }
  Serial.println("");

  // Connect to home station
  while (!client.connect(serverAddy, serverPort)) {
    Serial.println("[DEBUG] Failed to connect to server...");
    delay(1000);
  }
  Serial.println("[DEBUG] Connected to server!");

  // Send device type
  Serial.println("[DEBUG] Waiting for data.");
  while (!client.available()) {
    Serial.print(".");
  }
  Serial.println();

  char buff[1];
  client.read(buff, 1);
  for (auto x : buff) {
    Serial.println(x, HEX);
  }
  buff[0] = 0x01;
  client.print(buff);
  Serial.println("Device type sent!");

  // Establish keys with ECDH
  establishKeys();

  // Reset LED
  updateLED(none);

  delay(1000);
}

void loop() {
  if (client.available()) {
    recvMessage();
  }

  if (checkHeartbeat()) {
    updateLED(activeAlarm);
    enableAlarm(failedHeartbeat);
  }

  updateLED();
  checkResetBtn();
}
