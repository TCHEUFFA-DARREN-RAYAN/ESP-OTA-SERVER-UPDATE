#include <WiFi.h>
#include "esp_system.h"

#define GREEN_LED_PIN 2   // Define the pin for the green LED
#define RED_LED_PIN 4     // Define the pin for the red LED

// =====================
// WiFiManager Class
// =====================
class WiFiManager {
public:
    WiFiManager(const char* ssid, const char* password) {
        _ssid = ssid;
        _password = password;
    }

    void initializeWiFi() {
        Serial.begin(115200);
        delay(1000);

        Serial.println("Setting device name...");
        setDeviceName();  // Set the device name before connecting to Wi-Fi

        Serial.println("Connecting to Wi-Fi...");
        WiFi.begin(_ssid, _password);

        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.print(".");
        }

        Serial.println("\nWi-Fi connected.");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }

    bool isConnected() {
        return WiFi.status() == WL_CONNECTED;
    }

    void reconnectIfNeeded() {
        if (!isConnected()) {
            Serial.println("Wi-Fi lost. Reconnecting...");
            initializeWiFi();
        }
    }

private:
    const char* _ssid;
    const char* _password;

    void setDeviceName() {
        uint64_t macAddress = ESP.getEfuseMac();  // Get MAC address
        String macString = String((macAddress >> 40) & 0xFF, HEX) + ":" +
                           String((macAddress >> 32) & 0xFF, HEX) + ":" +
                           String((macAddress >> 24) & 0xFF, HEX) + ":" +
                           String((macAddress >> 16) & 0xFF, HEX) + ":" +
                           String((macAddress >> 8) & 0xFF, HEX) + ":" +
                           String(macAddress & 0xFF, HEX);

        String chipID = String(ESP.getChipModel());
        String deviceName = macString + ":" + chipID;

        WiFi.setHostname(deviceName.c_str());

        Serial.print("Device name set to: ");
        Serial.println(deviceName);
    }
};

// =====================
// Main Setup
// =====================
WiFiManager wifiManager("KING_HACKER", "kingsman");  // Replace with your credentials

void setup() {
    // Initialize Wi-Fi and Serial
    wifiManager.initializeWiFi();

    // Set up LED pins as output
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
}

void loop() {
    wifiManager.reconnectIfNeeded();  // Keep Wi-Fi connection alive

    // Green LED ON for 1 second
    digitalWrite(GREEN_LED_PIN, HIGH); 
    digitalWrite(RED_LED_PIN, LOW);    
    delay(1000);

    // Red LED ON for 1 second
    digitalWrite(GREEN_LED_PIN, LOW);  
    digitalWrite(RED_LED_PIN, HIGH); 
    delay(1000);
}
