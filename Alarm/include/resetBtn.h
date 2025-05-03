// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#ifndef RESETBTN_H
#define RESETBTN_H

#include <Arduino.h>
#include "eepromManager.h"

#define resetBtnPin D0

/*
 * @brief Checks the state of the reset button and takes action if required.
 *
 * @note This function should be run as often as possible to avoid delays.
 */
void checkResetBtn();

/*
 * @brief Sets the pin mode of the reset button to INPUT_PULLUP
 *
 */
void setupResetBtn();

#endif
