# Communication Protocol

## Overview
Device communication is handled via TCP sockets between a device and the central server. Upon initial connection the server will request from the device a device type then begin a device specific setup which includes a key exchange and any other necessary information. Communication continues to be handled using byte based messages.

## Device Type Identification
| **Purpose**                          | **Server indentifies connecting device type** |
|--------------------------------------|-----------------------------------------------|
| Communicate that the server is ready | `0xFF`                                        |
| Identifies what device is is         | 1-byte device type code                       |

#### Device Type Codes

| **Code**    | **Device**   |
|-------------|--------------|
| `0x01`      | Alarm Module |
| `0x02`      | Home Screen  |
| `0x03-0xFF` | Unallocated  |

## Message Format

| **Field**      | **Size**     | **Description**                                                     | **Value**   |
|----------------|--------------|---------------------------------------------------------------------|-------------|
| Header         | 2 Bytes      | Identify start of message                                           | `0xA5 5A`   |
| Message Type   | 1 Byte       | Identify type of message                                            | Dynamic     |
| Nonce          | 4 Bytes      | Unique number to prevent replay attacks                             | Dynamic     |
| Payload Length | 1 Byte       | Indicates how many blocks (16 bytes) of encrypted payload follow    | 0-255       |
| Payload        | 0-4080 Bytes | Encrypted payload containing data (Payload length * 16)             | AES-256     |
| HMAC           | 32 Bytes     | Message hash to ensure integrity. Includes everything except itself | SHA-256 Sum |

## Message Type

Indicates the purpose of the message. Message types are defined per device and can include actions such as heartbeat (`0x01`), alert (`0x02`), or update request (`0x03`).

For a full list of message types, refer to individual device specifications.

## HMAC Details

The HMAC (Hash-based Message Authentication Code) is calculated using the shared 16-byte key established during the ECDH key exchange process. It covers all message fields except the HMAC itself. This includes:
* Header
* Message Type
* Nonce
* Payload Length
* Payload (if present)

The HMAC is generated using SHA-256 and the message is uniquely transformed using the previously established key.

The purpose of this HMAC is to guarantee authenticity and integrity of each message. It ensures that the message has not been altered in transit, and that it originated from a trusted source. This protects the system from Man-in-the-Middle (MitM) and replay attacks, as any modification or forgery attempt will result in an invalid HMAC.

If a message fails HMAC verification, it is discarded immediately. In future implementations, failed messages will also be logged for offline analysis. This analysis may include:
* Investigating the intended message type
* Reviewing anomalies in frequency or pattern
* Correlating with other suspicious acitivty across devices
This approach strengthens system resilience by enabling post-incident forensics and early detection of sophisticated attacks

## Nonce Details

Each message includes a 4-byte nonce (number used once), which must be randomly generated and unique within the session.

This nonce plays a critical role in ensuring message freshness and protecting against replay attacks, where an attacker might attempt to resend a previously valid message.

This guarantees each message is fresh and uniquely identifiable, significantly reducing the risk of replay-based exploits.

## Key Exchange (ECDH)

Upon the initial device-server handshake, a secure key exchange is performed using Elliptic Curve Diffie-Hellman (ECDH). This generates a 32-byte key used for encryption and a 16-byte initialization vector (IV), which is used for both:
* IV for encrypting the payload with AES-256
* Computing the HMAC for authentication and integrity
  
This process ensures that even if the network is compromised, the key itself cannot be intercepted or derived.
