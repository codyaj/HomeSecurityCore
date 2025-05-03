// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#ifndef ALARMCONTROL_H
#define ALARMCONTROL_H

#include <Arduino.h>
#include "config.h"

enum alarmTriggers {
    noAlarm,
    failedHeartbeat,
    incommingAlert
};

/*
 * @brief Setup the pin used for the alarm
 *
 * Sets ALARM_PIN to output
 */
void setupAlarmPin();

/*
 * @brief Enables the alarm
 *
 * Sets ALARM_PIN to HIGH
 */
void enableAlarm(alarmTriggers trigger);

/*
 * @brief Disables the alarm
 *
 * Sets ALARM_PIN to LOW
 */
void disableAlarm(alarmTriggers trigger);

#endif
