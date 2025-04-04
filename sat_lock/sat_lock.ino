#include <TinyGPS++.h>
#include <HardwareSerial.h>

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);  // Use UART1 for ESP32, RX=16, TX=17

void setup() {
  Serial.begin(115200);         // Debugging via USB
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);  // RX=16, TX=17
  Serial.println("Testing GPS Module...");
}

void loop() {
  while (SerialGPS.available() > 0) {
    char c = SerialGPS.read();
    Serial.write(c);  // Output raw NMEA sentences to serial monitor
    if (gps.encode(c)) {
      if (gps.location.isUpdated()) {
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
      }
    }
  }

  // Check if GPS has acquired any satellite signal
  if (gps.satellites.isValid()) {
    Serial.print("Satellites in view: ");
    Serial.println(gps.satellites.value());
  } else {
    Serial.println("No satellites detected yet...");
  }
}
