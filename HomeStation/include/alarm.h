// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#ifndef ALARM_H
#define ALARM_H

#include "../include/device.h"

// #define INCOMMING_HEARTBEAT 0x00      - Completed automatically when message is received in parent class
#define OUTGOING_ENABLE_ALARM 0x01
#define OUTGOING_DISABLE_ALARM 0x02
#define OUTGOING_STATUS_REQUEST 0x03
#define OUTGOING_FIRMWARE_UPDATE 0x04
#define OUTGOING_TESTING_SEQUENCE 0x05
// #define OUTGOING_HEARTBEAT 0x00       - Completed automatically when message is received in parent class
#define INCOMMING_JAMMING_DETECTED 0x01
#define INCOMMING_STATUS_REPLY 0x03
#define INCOMMING_TAMPER_ALERT 0x04
#define INCOMMING_LOW_BATTERY 0x05

class Alarm : public Device{
private:
    bool alarmActive;
protected:
    /*
     *@brief Handles a incomming message for this device type.
     *@info Will be called only in the event that a received messages security is correct
     *
     *@param msgType - a byte value that is the value received in the message
     */
    void handleMessage(uint8_t msgType) override;

    /*
     *@brief Handles a incomming message for this device type.
     *@info Will be called only in the event that a received messages security is correct
     *
     *@param msgType - a byte value that is the value received in the message
     *       data    - a vector of bytes containing the messages data
     */
    void handleMessage(uint8_t msgType, std::vector<uint8_t> data) override;

public:
    /*
     *@brief Init the class
     *
     *@param clientSocket - The socket associated with this device
     */
    Alarm(int clientSocket);

    /*
     *@brief Checks if the alarm is active
     *
     *@return the value of the alarm (T/F)
     */
    bool isAlarmActive();

    virtual ~Alarm();
};

#endif // ALARM_H
