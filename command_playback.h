#ifndef COMMAND_PLAYBACK_H
#define COMMAND_PLAYBACK_H

// https://github.com/NicksonYap/digitalWriteFast
// FOR ARDUINO DUE, USE: https://github.com/lobbesnl/digitalWriteFast
#include <digitalWriteFast.h>

// #include "commands_fulldump.h"
// #include "commands_track_battery.h"
#include "commands_volume.h"

void setup() {
  pinModeFast(REMOTE_DATA_PIN, OUTPUT);
  digitalWriteFast(REMOTE_DATA_PIN, HIGH);

  common_setup();
}

// https://forum.arduino.cc/index.php?topic=125550.0

void sendCommandBuffer() {
  unsigned int commandLength = sizeof(commandBuffer);
  unsigned int currentLine = 0;
  for (unsigned int i = 0; i < commandLength; i++) {
    // if ((i % 100) == 0)
    //   Serial.print(".");
    // if ((i % 1000) == 0)
    //   Serial.println("");

    // if ((i % 1000) == 0) {
    //   Serial.print(i);
    //   Serial.print("/");
    //   Serial.println(commandLength);
    // }

    if ((currentLine % 10) == 0)
      Serial.println(currentLine);

    if (commandBuffer[i] < 0) {
      digitalWriteFast(REMOTE_DATA_PIN, LOW);
    } else {
      digitalWriteFast(REMOTE_DATA_PIN, HIGH);
    }

    unsigned short signalLength = abs(commandBuffer[i]);
    // if (signalLength < 5) {
    //   // https://forum.arduino.cc/index.php?topic=125550.0
    //   // "delayMicroseconds() waits at least 4us even if the parameter is less than 4."
    //   noInterrupts();
    //   for (uint8_t j = 0; j < signalLength; j++) {
    //     __asm__("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\t"); // about 1.2us delay on due
    //   }
    //   interrupts();

    // } else
    if (signalLength < 1000) {
      /*
      arduino.cc/reference/en/language/functions/time/delaymicroseconds/

      "Currently, the largest value that will produce an accurate delay is 16383.
      This could change in future Arduino releases. For delays longer than a few
      thousand microseconds, you should use delay() instead"
      */
      delayMicroseconds(signalLength);
    } else {
      delay(signalLength / 1000);
      // delayMicroseconds(signalLength - ((signalLength / 1000) * 1000));
    }

    if ((commandBuffer[i] > -1100) && (commandBuffer[i] < -900)) {
      currentLine++;
    }
  }
}

void loop(){
  sendCommandBuffer();

  digitalWriteFast(REMOTE_DATA_PIN, HIGH);
  Serial.print(micros()); Serial.println(" Looping");
  delay(1000);
}

#endif
