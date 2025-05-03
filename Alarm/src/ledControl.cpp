// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#include "ledControl.h"

indicatorModes currentMode;
unsigned long startTime;

namespace {
    void displayColour(colours colour, int brightnessVal) {
        if (brightnessVal > 255 || brightnessVal < 0) {
            Serial.println("[DEBUG] brightness value out of bounds");
            return;
        }

        // Normalize brightness from 0 to 1
        float brightness = brightnessVal / 255.0;

        // Init colour values
        uint8_t redVal;
        uint8_t greenVal;
        uint8_t blueVal;


        switch (colour) {
            case orange: {
                redVal = 255 * brightness;
                greenVal = 165 * brightness;
                blueVal = 0 * brightness;
                break;
            }
            case lightRed: {
                redVal = 255 * brightness;
                greenVal = 51 * brightness;
                blueVal = 51 * brightness;
                break;
            }
            case red: {
                redVal = 255 * brightness;
                greenVal = 0 * brightness;
                blueVal = 0 * brightness;
                break;
            }
            case lightGreen: {
                redVal = 144 * brightness;
                greenVal = 238 * brightness;
                blueVal = 144 * brightness;
                break;
            }
            case green: {
                redVal = 0 * brightness;
                greenVal = 255 * brightness;
                blueVal = 0 * brightness;
                break;
            }
            case yellow: {
                redVal = 255 * brightness;
                greenVal = 255 * brightness;
                blueVal = 0 * brightness;
                break;
            }
            case white: {
                redVal = 255 * brightness;
                greenVal = 255 * brightness;
                blueVal = 255 * brightness;
                break;
            }
            case blue: {
                redVal = 0 * brightness;
                greenVal = 128 * brightness;
                blueVal = 255 * brightness;
                break;
            }
        }

        analogWrite(RED_PIN, redVal);
        analogWrite(GREEN_PIN, greenVal);
        analogWrite(BLUE_PIN, blueVal);
    }

    void displayColour(uint8_t redVal, uint8_t greenVal, uint8_t blueVal) {
        analogWrite(RED_PIN, redVal);
        analogWrite(GREEN_PIN, greenVal);
        analogWrite(BLUE_PIN, blueVal);
    }

    void displayConnectivityStrength() {
        // Get RSSI Between 0 and 120
        int rssi = WiFi.RSSI() * -1;

        // ! COLOUR TESTING REMOVE
        //int rssi = (millis() / 100) % 120;

        int redVal = rssi * 2.125;
        int greenVal = (120 - rssi) * 2.125;

        displayColour(redVal, greenVal, 0);
    }
}

void updateLED() {
    if (currentMode == none) {
        return;
    }

    switch (currentMode) {
        case setupMode: { // Pulsing yellow
            // Calculate the pulsing brightness using sine wave
            float sineWave = sin(2 * PI * PULSE_FREQUENCY * (millis() / 1000.0));  // Sine wave based on time
            int brightness = int((sineWave + 1) * 127.5 + 0.5); // Normalize and scale to 0-255

            // Display the color with calculated brightness
            displayColour(yellow, brightness);

            break;
        }
        case activeAlarm: { // Flashing Red

            if ((millis() % FLASH_PERIOD) < (FLASH_PERIOD / 2)) {
                // On
                displayColour(red, 255);
            } else {
                // Off
                displayColour(red, 0);
            }

            break;
        }
        case alarmDisabled: { // Fading green
            unsigned long timeDiff = millis() - startTime;
            
            if (timeDiff > FADING_LENGTH) {
                displayColour(green, 0);
                currentMode = none;
                break;
            }
            int brightness = (((FADING_LENGTH - timeDiff) / (FADING_LENGTH * 1.0)) * 255) + 0.5;

            displayColour(green, brightness);

            break;
        }
        case lowBattery: { // Pulsing lightRed
            // Calculate the pulsing brightness using sine wave
            float sineWave = sin(2 * PI * PULSE_FREQUENCY * (millis() / 1000.0));  // Sine wave based on time
            int brightness = int((sineWave + 1) * 127.5 + 0.5); // Normalize and scale to 0-255

            // Display the color with calculated brightness
            displayColour(lightRed, brightness);
            break;
        }
        case connectionStrength: {
            displayConnectivityStrength();
            break;
        }
        case alarmArmed: { // Fading Red
            unsigned long timeDiff = millis() - startTime;
            
            if (timeDiff > FADING_LENGTH) {
                displayColour(green, 0);
                currentMode = none;
                break;
            }
            int brightness = (((FADING_LENGTH - timeDiff) / (FADING_LENGTH * 1.0)) * 255) + 0.5;

            displayColour(red, brightness);
            break;
        }
        case waiting: { // Pulsing blue
            // Calculate the pulsing brightness using sine wave
            float sineWave = sin(2 * PI * PULSE_FREQUENCY * (millis() / 1000.0));  // Sine wave based on time
            int brightness = int((sineWave + 1) * 127.5 + 0.5); // Normalize and scale to 0-255

            // Display the color with calculated brightness
            displayColour(blue, brightness);
            break;
        }
        case none: {
            displayColour(red, 0);
            break;
        }
    }
}

void updateLED(indicatorModes newMode) {
    startTime = millis();
    currentMode = newMode;
}

void setupLED() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}
