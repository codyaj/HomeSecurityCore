// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#ifndef MAIN_H
#define MAIN_H

#include "../include/alarm.h"
#include "../include/homeScreen.h"
#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 19001

// Device Identifiers
#define DEVICE_ALARM 0x01
#define DEVICE_SCREEN 0x02

#endif
