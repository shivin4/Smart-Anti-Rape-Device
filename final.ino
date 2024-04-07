#include <SoftwareSerial.h>

#define FORCE_SENSOR_PIN A0
#define BUTTON_1_PIN 2
#define BUTTON_2_PIN 3

SoftwareSerial gsmSerial(8, 9); // RX, TX pins for GSM module

bool emergencyDetected = false;
bool faultAlarmStopped = false;
unsigned long startTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(FORCE_SENSOR_PIN, INPUT);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  gsmSerial.begin(9600);

  // Print setup completion message
  Serial.println("Arduino setup completed.");
}

void loop() {
  int forceValue = analogRead(FORCE_SENSOR_PIN);
  
  if (forceValue > 20 || digitalRead(BUTTON_1_PIN) == LOW) {
    startTime = millis(); // Record the start time
    
    Serial.println("Emergency detected, waiting for 10 seconds...");
    while ((millis() - startTime) < 10000) { // Wait for 10 seconds or until button 2 is pressed
      if (!digitalRead(BUTTON_2_PIN)) {
        Serial.println("Fault alarm stopped.");
        faultAlarmStopped = true;
        break;
      }
    }
    
    if (!faultAlarmStopped && (millis() - startTime) >= 10000) {
      Serial.println("Emergency detected! Sending location message and calling emergency contact.");
      Serial.println("Current location: Latitude 28.4497859, Longitude 77.5829657");
      // Send current location message to a number
      sendLocationMessage();
      // Call emergency contact with a recorded message
      callEmergencyContact();
      emergencyDetected = true;
    }
  }
}

void sendLocationMessage() {
  // Code to send current location message
  Serial.println("Sending current location message...");
  gsmSerial.println("AT+CMGS=\"+919000283611\"\r");
  gsmSerial.println("Hi Archana here I am in unsafe location please reach out. Current location: Latitude 28.4497859, Longitude 77.5829657");
} 

void callEmergencyContact() {
  // Code to call emergency contact
  Serial.println("Calling emergency contact...");
  gsmSerial.println("ATD+918287858458;\r");
}
