// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#include "../include/main.h"

bool s_Finished = false;

void handleAlarm(int clientSocket) {
    Alarm alarm(clientSocket);
    while (!s_Finished) {
        // Uncomment for test message sending every 2 seconds
        // std::vector<uint8_t> emptyVector;
        // alarm.sendMessage(0xAA, emptyVector);
        // std::this_thread::sleep_for(std::chrono::seconds(2));
        if (alarm.isDataAvailable()) {
            alarm.receiveMessage();
        }
    }
    close(clientSocket);
}

void handleScreen(int clientSocket) {
    HomeScreen homeScreen(clientSocket);
    while (!s_Finished) {
        if (homeScreen.isDataAvailable()) {
            homeScreen.receiveMessage();
        }
    }
    close(clientSocket);
}

uint8_t getDeviceType(int clientSocket) {
    std::cout << "Sending 0xFF\n";
    uint8_t msg = 0xFF;
    send(clientSocket, &msg, 1, 0);
        
    // Receive device type from client
    uint8_t buffer[1024] = {0};
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        std::cout << "Client disconnected or error occurred. Exiting thread.\n";
        return 0xFF;
    }
    std::cout << buffer[0] << " <-- device type\n";
    return buffer[0];
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM , 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces

    std::cout << "Server socket created and initialized\n";

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        return -1;
    }

    std::cout << "Server socket bound to address and port\n";

    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed");
        return -1;
    }

    std::cout << "Listening for incoming connections...\n";
    
    std::vector<std::thread> deviceThreads;   

    while (1) {
        // Wait for a device to connect
        int clientSocket = accept(serverSocket, nullptr, nullptr);

        std::cout << "Connection received\n";

        // Identify the connected device type
        uint8_t deviceType = getDeviceType(clientSocket);

        std::cout << "Device type: " << (int)deviceType << "\n";

        // Spawn a handler thread based on device type
        switch (deviceType) {
            case DEVICE_ALARM:
                deviceThreads.emplace_back(std::thread(handleAlarm, clientSocket));
                break;
            case DEVICE_SCREEN:
                deviceThreads.emplace_back(std::thread(handleScreen, clientSocket));
                break;
            default:
                std::cout << "Unknown device type. Ignoring connection\n";
                close(clientSocket);
        }
    }

    // Join all handler threads before exit (unreachable in this loop structure)
    for (auto& thread : deviceThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
