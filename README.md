# HomeSecurityCore
A modular, scalable, and cyber-secure home security system built with C++, featuring core modules for alarm and home station, designed for future extensibility.

This home security system is a secure, self-hosted home system designed to run entirely on a local network. It supports modular devices (alarm modules, control panels, etc. ) that securely connect to a central server using a custom encrypted communication protocol

## Purpose
This repository serves as a **proof of concept** for a self-hosted, secure home security system that could potentially be commercialized in the future. The system demonstrates the feasibility of creating a fully localized, modular home security solution that does not rely on cloud services. 

While still in development, this project is an important part of my personal portfolio and is being built with the intention to eventually offer it as a commercial product. The system showcases a secure communication protocol and a flexible, expandable architecture that can be adapted to various home security needs.

## Features
- **Local-only network communication** (no cloud dependencies)
- **Ironclad messaging protocol** using ECDH key exchange, AES-256 encryption, nonces, and SHA-256 HMAC's.
- **Modular architecture** supporting multiple device types
- **Easy to expand and customize**

## System Architecture
For full technical details on system design and communication, see the `architecture/` folder:
- [System Overview](architecture/overview.md)
- [Communication Protocol](architecture/protocol.md)

## License

This project is licensed under Creative Commons Attribution-NonCommercial 4.0 International Public License.

[Read more here](LICENSE)
