# ESP32 WiFi + Bluetooth GPS Tracker

This project uses an **ESP32** and a **NEO-6M GPS module** to track and send GPS coordinates via **WiFi** to Google Sheets and via **Bluetooth** to nearby devices.

---

## Features

- Tracks real-time **latitude and longitude**
- Sends GPS coordinates to a **Google Sheets log** using **Apps Script**
- Sends GPS data via **Bluetooth** to paired devices (e.g. Android)
- Displays **satellite count** for diagnostics

---

## Hardware Used

- **ESP32-WROOM-32D**
- **NEO-6M GPS Module**
- Li-ion Battery or USB Power
- Wires (connected GPS TX → ESP32 GPIO12, GPS RX → GPIO13)

---

## Wiring

| GPS Module | ESP32 |
|------------|-------|
| TX         | GPIO12 |
| RX         | GPIO13 |
| VCC        | 3.3V |
| GND        | GND |

---

## Setup Instructions

1. Clone this repository and open the `.ino` files in Arduino IDE.
2. The sat_lock.ino file is to test the gps module and the googlesheet_map.ino is the main code.
3. Install the following libraries:
   - `TinyGPS++`
   - `BluetoothSerial`
   - `HTTPClient`
4. Replace your **WiFi credentials**:
   ```cpp
   const char* ssid = "your_wifi_ssid";
   const char* password = "your_wifi_password";

## How the data flow works

GPS → ESP32 → 
    ├── Bluetooth → Phone
    └── WiFi → Google Sheets (via Apps Script)

    
The ESP32 creates this link:
https://maps.google.com/?q=LATITUDE,LONGITUDE


Google Apps Script (for Google Sheets)
1. Go to script.google.com and create a new project.
2. Replace the code with the following:
   
  function doPost(e) {
  var sheet = SpreadsheetApp.openById('YOUR_SPREADSHEET_ID').getSheets()[0];

  var data = JSON.parse(e.postData.contents);
  var lat = data.latitude;
  var lng = data.longitude;
  var time = new Date();
  var mapsLink = "https://maps.google.com/?q=" + lat + "," + lng;

  sheet.appendRow([time, lat, lng, mapsLink]);

  return ContentService.createTextOutput("Success").setMimeType(ContentService.MimeType.TEXT);
}


3. Deploy it as a web app (select: Execute as: Me, Anyone can access).
4. Copy the deployment URL and paste it into the Arduino code.
