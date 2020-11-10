/*
  This example demonstrate pH value reading
*/
#include "Arduino.h"

// Include library
#include "TroykaPH.h"

TroykaPH phMeter(A4); // set used analog pin

uint32_t showingTime;
constexpr uint32_t INTERVAL = 3000;

void setup() {
    phMeter.begin(); // if module not calibrated or...
    // phMeter.begin(correction,zeroShift); // if you have it (use calibrate.ino) for it

    Serial.begin(9600);
    showingTime = millis() + INTERVAL; // show result once per 3 seconds
}

void loop() {
    phMeter.update(1000); // real read from sensor once per second
    // (you can increase this period, in practice pH value changing too slowly)

    if (millis() - showingTime > INTERVAL) {
        Serial.print("\nCurrent pH: ");
        Serial.print(phMeter.read(), 1);
        showingTime += INTERVAL;
    }
}
