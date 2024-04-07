#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define FORCE_SENSOR_PIN A0
#define BUTTON_1_PIN 2
#define BUTTON_2_PIN 3

SoftwareSerial gsmSerial(8, 9); // RX, TX pins for GSM module
SoftwareSerial gpsSerial(10, 11); // RX, TX pins for GPS module

TinyGPSPlus gps;

bool emergencyDetected = false;
bool faultAlarmStopped = false;

void setup() {
  Serial.begin(9600);
  pinMode(FORCE_SENSOR_PIN, INPUT);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  gsmSerial.begin(9600);
  gpsSerial.begin(9600);

  // Print setup completion message
  Serial.println("Arduino setup completed.");
}

void loop() {
  int forceValue = analogRead(FORCE_SENSOR_PIN);
  
  if (forceValue > 50 || digitalRead(BUTTON_1_PIN) == LOW) {
    Serial.println("Emergency detected, waiting for 10 seconds...");
    delay(10000); // Wait for 10 seconds or until button 2 is pressed
    
    if (!faultAlarmStopped) {
      Serial.println("Fault alarm stopped.");
      // Perform actions for fault alarm stopped
    } else {
      if (!digitalRead(BUTTON_2_PIN)) {
        Serial.println("Emergency call cancelled.");
        faultAlarmStopped = true;
      } else {
        Serial.println("Emergency detected! Sending location message and calling emergency contact.");
        // Send current location message to a number
        sendLocationMessage();
        // Call emergency contact with a recorded message
        callEmergencyContact();
        emergencyDetected = true;
      }
    }
  }

  // Read GPS data and update location
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        Serial.print("Latitude: ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(" Longitude: ");
        Serial.println(gps.location.lng(), 6);
      } else {
        Serial.println("Location data not available.");
      }
    }
  }
}

void sendLocationMessage() {
  // Code to send current location message
  Serial.println("Sending current location message...");
  gsmSerial.println("AT+CMGS=\"+919999999999\"\r");
  gsmSerial.print("Hi Archana here I am in unsafe location please reach out. Current location: ");
  gsmSerial.print("Latitude: ");
  gsmSerial.print(gps.location.lat(), 6);
  gsmSerial.print(" Longitude: ");
  gsmSerial.print(gps.location.lng(), 6);
  gsmSerial.println((char)26); // End the message with Ctrl+Z character
} 

void callEmergencyContact() {
  // Code to call emergency contact
  Serial.println("Calling emergency contact...");
  gsmSerial.println("ATD+919999999999;\r");
}
