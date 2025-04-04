#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "BluetoothSerial.h"

// WiFi Credentials
const char* ssid = "wifi-ssid";
const char* password = "you-password";

// Google Apps Script URL
const char* serverName = "https://script.google.com/macros/s/your_script_id_here/exec";

// GPS + Bluetooth
TinyGPSPlus gps;
HardwareSerial SerialGPS(1);
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_GPS");
  SerialGPS.begin(9600, SERIAL_8N1, 12, 13);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  while (SerialGPS.available()) {
    gps.encode(SerialGPS.read());
  }

  if (gps.location.isUpdated()) {
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();
    String mapsLink = "https://maps.google.com/?q=" + String(latitude, 6) + "," + String(longitude, 6);

    // Bluetooth Output
    SerialBT.print("Latitude: ");
    SerialBT.print(latitude, 6);
    SerialBT.print(", Longitude: ");
    SerialBT.print(longitude, 6);
    SerialBT.print("\nGoogle Maps Link: ");
    SerialBT.println(mapsLink);

    // Send to Google Sheets
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String postData = "{\"latitude\":" + String(latitude, 6) + ",\"longitude\":" + String(longitude, 6) + "}";
    int httpResponseCode = http.POST(postData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    SerialBT.print("HTTP Response: ");
    SerialBT.println(httpResponseCode);

    http.end();
  }

  // Satellite count
  if (gps.satellites.isValid()) {
    SerialBT.printf("Satellites in view: %d\n", gps.satellites.value());
  } else {
    SerialBT.println("No satellites detected yet...");
  }

  delay(1000);
}
