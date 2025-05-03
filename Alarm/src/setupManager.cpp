// Home Security Core – Proof of Concept
// © 2025 Cody Jackson. All rights reserved.
// This file is part of the Home Security Lite release.
// Intended for educational and demonstration purposes only. Not for commercial use.
// DISCLAIMER: This is a work-in-progress. Some features are incomplete or not yet implemented.

#include "setupManager.h"

DNSServer dnsServer;
AsyncWebServer server(80);

bool detailsReceived = false;

const char indexHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Alarm | Sentro Security</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            background-color: #f4f4f9;
            margin: 0;
            padding: 0;
            display: flex;
            flex-direction: column;
            justify-content: center;
            height: 100vh;
        }
        .container {
            max-width: 500px;
            margin: auto;
            padding: 20px;
            background: #ffffff;
            border-radius: 8px;
            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
        }
        h1 {
            color: #333;
        }
        p {
            color: #666;
        }
        form {
            margin-top: 20px;
        }
        input[type="text"], input[type="password"], input[type="email"] {
            width: 90%;
            padding: 10px;
            margin: 10px 0;
            border: 1px solid #ccc;
            border-radius: 5px;
        }
        button {
            background: #1c63e7;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 5px;
            cursor: pointer;
        }
        button:hover {
            background: #4d85ec;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Welcome to Home Security</h1>
        <p>Please enter the details on the back of your home station.</p>
        <form action="/get">
            <input type="text" name="SSID" placeholder="Enter SSID">
            <input type="password" name="password" placeholder="Enter password">
            <button type="submit">Connect</button>
        </form>
    </div>
</body>
</html>)rawliteral";

class CaptiveRequestHandler : public AsyncWebHandler {
public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request) {
        return true;
    }    

    void handleRequest(AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", indexHTML);
    }
};

namespace {
    void setupAP() {
        WiFi.softAP(setupSSID, setupPass);
        Serial.print("[DEBUG] IP: ");
        Serial.println(WiFi.softAPIP());
    }

    void setupServer() {
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send_P(200, "text/html", indexHTML);
            Serial.println("[DEBUG] Client Connected");
        });

        // ? Implement rate limit ??
        server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
            String inputSSIDString;
            String inputPasswordString;

            if (request->hasParam("SSID") && request->hasParam("password")) {
                inputSSIDString = request->getParam("SSID")->value();
                inputPasswordString = request->getParam("password")->value();

                Serial.println("[DEBUG] Details entered: ");
                Serial.println(inputSSIDString);
                Serial.println(inputPasswordString);
                
                // Verify Input
                if (inputSSIDString.length() > 32 || inputPasswordString.length() < 8 || inputPasswordString.length() > 63) {
                    request->send(400, "text/html", "Invalid details! Please check your input.");
                    return;
                }

                // Convert strings to char*
                char inputSSID[32];
                char inputPassword[63];

                inputSSIDString.toCharArray(inputSSID, 32);
                inputPasswordString.toCharArray(inputPassword, 63);

                // Save credentials to EEPROM
                saveWiFiCredentials(inputSSID, inputPassword);

                // Send client confirmation
                request->send(200, "text/html", "Details received!");
                detailsReceived = true;
            }

            request->send(400, "text/html", "Invalid details! Please check your input.");
        });
    }
}

// Make esp an AP
void beginSetup() {
    // Change LED
    setupAP();
    setupServer();
    dnsServer.start(53, "*", WiFi.softAPIP());
    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only when requested from AP
    server.begin();
    Serial.println("[DEBUG] Server Setup!");

    // ? Stay in this loop until the EEPROM has been entered
    while (!detailsReceived) {
        dnsServer.processNextRequest();
        updateLED();
    }
}


