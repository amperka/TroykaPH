/*
  This example demonstrate pH value reading
*/
#include "Arduino.h"


// Include library
#include "troykaph.h"

TroykaPH phMeter(A4); // set used analog pin

uint32_t showingTime;

void setup() {
  phMeter.begin(); // if not calibrated or...
  // phMeter.begin(calibrationFactor); // if you have it (use calibrate.ino) for it

  Serial.begin(9600);
  showingTime = millis() + 3000; // show result once per 3 seconds
}

void loop() {
  phMeter.update(1000); // real read from sensor once per second
  // (you can increase this period, in practice pH value changing too slowly)

  if (showingTime <= millis()) {
    Serial.print("\nCurrent pH: ");
    Serial.print(phMeter.read());
    showingTime += 3000;
  }
}
