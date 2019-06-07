#include "Arduino.h"

void setup() {
    for (int i=0;i<24;i++) {
        pinMode(i, INPUT_PULLUP);
    }

	Serial.begin(115200);
}

void loop() {
    delay(1000);
    String status;
    for (int i=0;i<24;i++) {
        status += String(i) + ": " + String(digitalRead(i)) + "\t";
    }
    Serial.println(status);
}