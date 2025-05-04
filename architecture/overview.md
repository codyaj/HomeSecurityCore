# Overview

## System Overview

This is a distributed home security system designed to operate on an isolated local network, ensuring that all communication remains secure and self-contained. The system connects various security devices (e.g., alarm modules, home screens) to a central server, enabling centralized monitoring and control.

## Core Principles

The core principles of this project are:
* **Security Focused:**
  - This system employs strong encryption, secure key exchange, and message verification to ensure that all communications are protected from potential attacks
  - The use of Elliptic Curve Diffie-Hellman (ECDH) for key exchange, AES-256 for encryption, and HMAC for message integrity ensures a high level of security.
* **No cloud costs:**
  - All communication occurs on an isolated local network, removing reliance on cloud services and associated costs. This ensures that the system is fully self-sufficient and private.
* **Fully controllable:**
  - The system is entirely under the user’s control. The central server and connected devices allow for real-time monitoring and adjustments from any authorized device, providing flexibility and full control over security settings.
* **Easily expandable:**
  - The system has been designed to allow easy addition of new devices. As new security modules are introduced, they can be seamlessly integrated into the existing network without major system overhauls.
* **Hassle-Free:**
  - Setup and operation are intuitive and straightforward. The system is designed to run with minimal manual intervention once set up, providing peace of mind and reducing the maintenance burden for the user.

## Core Components

* **Central Server:**
  - Acts as the hub for all device communications
  - Handles device type identification, secure key exchanges, and message routing.
  - Processes and stores incoming data from devices (e.g., alerts, status updates).
* **Devices:**
  - **Alarm Module:**
    * Responsible for alerting residents and deterring intruders through audible alarms (e.g., sirens) and visual cues.
    * Provides status indication via lighting (e.g., flashing LEDs) to communicate system states such as "armed", "triggered", or "disarmed".
    * Functions as both a notification mechanism and a deterrent, playing a critical role in immediate incident response.
  - **Home Screen:**
    * Serves as the primary user interface for interacting with the system.
    * Allows users to:
      - Configure settings
      - View live camera feeds
      - Review past and active alerts
      - Examine recent logs and suspicious activity
      - Arm and disarm the alarm system
    * Designed to be intuitive and comprehensive, providing full control and visibility over the system’s operations.
  - **Future Expansion:**
    * The architecture is designed to be modular and extensible
    * Planned future device integrations include:
      - Environmental and hazard sensors
      - Entry and motion sensors
      - Smart access controls
      - Visual monitoring modules
    * All additions will follow the same security-first, local-first design principles.
