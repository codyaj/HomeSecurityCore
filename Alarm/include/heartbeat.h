// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <Arduino.h>
#include <Crypto.h>
#include "messageUtils.h"
#include "ledControl.h"
#include "alarmControl.h"

// Time between sending last heartbeat and sending next
#define HEARTBEAT_INTERVAL 4000

// Max acceptable time between sending heartbeat and receiving 
#define HEARTBEAT_WAIT 2000

#define MAX_SEND_ATTEMPTS 3

/*
 * @brief Checks the last time the heartbeat was received.
 *
 * @return True if the heartbeat has not been received in time. False otherwise.
 */
bool checkHeartbeat();

/*
 * @brief Updates the last time the heartbeat was received
 *
 */
void receivedHeartbeat();

#endif
