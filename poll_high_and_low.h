#ifndef POLL_HIGH_AND_LOW_H
#define POLL_HIGH_AND_LOW_H

// https://github.com/NicksonYap/digitalWriteFast
// FOR ARDUINO DUE, USE: https://github.com/lobbesnl/digitalWriteFast
#include <digitalWriteFast.h>

void setup() {
  pinModeFast(REMOTE_DATA_PIN, INPUT);

  common_setup();
}

/* --- polling for high and low state times --- */

String frameBuffer = "";

void loop() {
  unsigned long currentLowStartedAt = micros();
  while (digitalReadFast(REMOTE_DATA_PIN) == LOW) {
    // while (booleanAvgRead(REMOTE_DATA_PIN, 5) == LOW) {
  }

  unsigned long currentLowEndedAt = micros();

  unsigned long currentLowLength = currentLowEndedAt - currentLowStartedAt;
  if (currentLowLength > FRAME_BEGIN_PULSE_MICROS)
  {
    Serial.println(frameBuffer);
    frameBuffer = "";
  }

  frameBuffer.concat("-");
  frameBuffer.concat(currentLowLength);
  frameBuffer.concat(", ");

  unsigned long currentHighStartedAt = micros();
  while (digitalReadFast(REMOTE_DATA_PIN) == HIGH) {
    // while (booleanAvgRead(REMOTE_DATA_PIN, 5) == HIGH) {
  }

  unsigned long currentHighEndedAt = micros();

  unsigned long currentHighLength = currentHighEndedAt - currentHighStartedAt;
  frameBuffer.concat("+");
  frameBuffer.concat(currentHighLength);
  frameBuffer.concat(", ");
}

#endif
