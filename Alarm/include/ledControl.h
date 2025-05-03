// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#define RED_PIN D5
#define GREEN_PIN D6
#define BLUE_PIN D7
#define PULSE_FREQUENCY 0.5 // Pulses per second
#define FLASH_FREQUENCY 1   // Flash Freq (50% duty cycle)
#define FLASH_PERIOD (1000 / FLASH_FREQUENCY)
#define FADING_LENGTH 4000  // Length in ms to fade for

#include <Arduino.h>
#include "config.h"
#include "ESP8266WiFi.h"

enum indicatorModes {
    none,
    setupMode,            // Pulsing Yellow
    activeAlarm,          // Flashing Red
    alarmDisabled,        // Fading green
    lowBattery,           // Pulsing lightRed
    connectionStrength,   // 
    alarmArmed,           // Fading Red
    waiting               // Pulsing Blue
};

enum colours {
    orange,
    lightRed,
    red,
    lightGreen,
    green,
    yellow,
    white,
    blue
};

/*
 * @brief Sets the RGB pins to output
 *
 */
void setupLED();

/*
 * @brief Updates the LEDs for features like fading
 *
 * @note This function should be called as frequently as possible to allow for smooth colour transitions
 */
void updateLED();

/*
 * @brief Updates the mode the LEDs are in
 *
 * @param[indicatorModes] The mode you wish to set the LEDs too. (input)
 */
void updateLED(indicatorModes);

#endif
