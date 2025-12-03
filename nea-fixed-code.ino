// ----------------------------------------------------------
//                          GCSE-DT-NEA
// ----------------------------------------------------------
// v1.2 – Digital Training Station Fix (Debounce + Clean Trigger)
// Iftekhar Syed, z3nxth, www.iftekhar.rocks/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define LEDPIN 5
#define SENSORPIN 4

Adafruit_7segment matrix = Adafruit_7segment();

int lastState = HIGH;
int score = 0;

unsigned long lastTrigger = 0;
const unsigned long debounceTime = 150; // ms

void setup() {
  Serial.begin(9600);
  Serial.println("Booted");

  matrix.begin(0x70);

  pinMode(LEDPIN, OUTPUT);
  pinMode(SENSORPIN, INPUT_PULLUP);

  digitalWrite(LEDPIN, LOW);
}

void loop() {

  int sensorState = digitalRead(SENSORPIN);

  // Edge detection: HIGH -> LOW means beam just broke
  if (sensorState == LOW && lastState == HIGH) {
    unsigned long now = millis();

    // Simple debounce
    if (now - lastTrigger > debounceTime) {
      score++;
      digitalWrite(LEDPIN, HIGH);

      Serial.println("Broken");
      lastTrigger = now;
    }
  }

  if (sensorState == HIGH && lastState == LOW) {
    Serial.println("Unbroken");
    digitalWrite(LEDPIN, LOW);
  }

  lastState = sensorState;

  // Update display
  matrix.println(score);
  matrix.writeDisplay();

  delay(5); // smooth but fast loop
}
