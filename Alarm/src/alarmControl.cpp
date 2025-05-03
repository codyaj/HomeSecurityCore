// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#include "alarmControl.h"

alarmTriggers currAlarm;

void setupAlarmPin() {
  pinMode(ALARM_PIN, OUTPUT);
}

void enableAlarm(alarmTriggers trigger) {
  digitalWrite(ALARM_PIN, HIGH);
  currAlarm = trigger;
  Serial.println("[DEBUG] Alarm Enabled");
}

void disableAlarm(alarmTriggers trigger) {
  if (currAlarm == noAlarm) {
    return;
  } else if (trigger == failedHeartbeat && currAlarm == incommingAlert) {
    Serial.println("[DEBUG] Unable to disable alarm. Difference in levels between failed heartbeat and incomming alert!");
    return;
  }

  digitalWrite(ALARM_PIN, LOW);
  currAlarm = noAlarm;
  Serial.println("[DEBUG] Alarm Disabled");
}
