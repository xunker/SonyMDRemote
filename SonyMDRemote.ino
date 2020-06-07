
#include "messages.h"

#define REMOTE_DATA_PIN A0
// If using DigitalWriteFast:
// int pinNum = 9; //do not use variables, macro will revert to the slower pinMode()
// const int pinNum = 9; //this is a constant, will use port manipulation (fast)

// setup commands common to all approaches, to be called within individual setup() routines
void common_setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  Serial.println("Ready");
}

unsigned long wentLowAt = 0;
unsigned long wentHighAt = 0;

#define FRAME_BEGIN_PULSE_MICROS 800

unsigned int toNearest(uint8_t nearest, unsigned int num) {
  return ((num + (nearest / 2)) / nearest) * nearest;
}

boolean booleanAvgRead(uint8_t pin, uint8_t reads) {
  uint8_t highReads = 0;
  uint8_t lowReads = 0;
  for (uint8_t i = 0; i < reads; i++)
  {
    if (digitalRead(pin))
    {
      highReads++;
    }
    else
    {
      lowReads++;
    }
    return (highReads >= lowReads);
  }
}

/* Uncommend ONLY ONE of the following includes */
// #include "poll_high_and_low.h"
#include "command_playback.h"

// boolean booleanAvgRead(uint8_t pin, uint8_t reads)
// {
//   uint8_t highReads = 0;
//   uint8_t lowReads = 0;
//   for (uint8_t i = 0; i < reads; i++) {
//     if (digitalRead(pin)) {
//       highReads++;
//     } else {
//       lowReads++;
//     }
//     return (highReads >= lowReads);
//   }
// }



  /* --- polling for low times only and rejecting data that does fit out pattern --- */

  // String frameBuffer = "";
  // unsigned long currentLowStartedAt = 0;
  // unsigned long currentLowEndedAt = 0;
  // unsigned long currentHighStartedAt = 0;
  // unsigned long currentHighEndedAt = 0;

  // void loop() {
  //   while (digitalRead(REMOTE_DATA_PIN) == LOW) {
  //     if (currentLowStartedAt == 0)
  //       currentLowStartedAt = micros();
  //     if (currentHighStartedAt > 0)
  //       currentHighStartedAt = 0;
  //   }

  //   currentLowEndedAt = micros();

  //   unsigned long currentLowLength = currentLowEndedAt - currentLowStartedAt;
  //   boolean framestart = false;
  //   if ((currentLowLength > FRAME_BEGIN_PULSE_MICROS) && (currentLowLength <= FRAME_BEGIN_PULSE_MICROS * 2)){
  //     Serial.println(frameBuffer);
  //     frameBuffer = "";
  //     framestart = true;
  //   }

  //   // frameBuffer.concat("_");
  //   // frameBuffer.concat(currentLowLength);
  //   // frameBuffer.concat(" ");

  //   while (digitalRead(REMOTE_DATA_PIN) == HIGH) {
  //     if (currentHighStartedAt == 0)
  //       currentHighStartedAt = micros();
  //     if (currentLowStartedAt > 0)
  //       currentLowStartedAt = 0;
  //   }

  //   currentHighEndedAt = micros();

  //   unsigned long currentHighLength = currentHighEndedAt - currentHighStartedAt;
  //   // frameBuffer.concat("-");
  //   // frameBuffer.concat(currentHighLength);
  //   // frameBuffer.concat(" ");

  //   if ((!framestart) && (currentLowLength <= FRAME_BEGIN_PULSE_MICROS * 2)) {
  //     if (currentLowLength <= 20) {
  //       frameBuffer.concat(1);
  //     } else if (currentLowLength <= 230) {
  //       frameBuffer.concat(0);
  //     } else {
  //       frameBuffer.concat("_");
  //       frameBuffer.concat(currentLowLength);
  //       frameBuffer.concat("-");
  //       frameBuffer.concat(currentHighLength);
  //       frameBuffer.concat(" ");
  //     }
  //   }
  // }

  /* --- pulsein --- */

  // String frameBuffer = "";
  // unsigned long currentPulseEndedAtMicros = 0;
  // unsigned long previousPulseEndedAtMicros = 0;

  // void loop() {
  //   unsigned int pulseLengthMicros = pulseIn(REMOTE_DATA_PIN, LOW);
  //   if (pulseLengthMicros > 0) {
  //     currentPulseEndedAtMicros = micros();

  //     if (pulseLengthMicros > FRAME_BEGIN_PULSE_MICROS) {
  //       Serial.println(frameBuffer);
  //       frameBuffer = "";

  //       // frameBuffer = "";
  //       // frameBuffer.concat(pulseLengthMicros);
  //       // frameBuffer.concat(" ");
  //     } else {

  //       // // frameBuffer.concat(pulseLengthMicros);
  //       // // frameBuffer.concat("-");
  //       // if (pulseLengthMicros > 170) {
  //       //   frameBuffer.concat(0);
  //       // } else {
  //       //   frameBuffer.concat(1);
  //       // }
  //     }

  //     frameBuffer.concat(toNearest(10, pulseLengthMicros));
  //     if (pulseLengthMicros < FRAME_BEGIN_PULSE_MICROS) {
  //       frameBuffer.concat("/");
  //       frameBuffer.concat(toNearest(10, currentPulseEndedAtMicros - previousPulseEndedAtMicros));
  //       frameBuffer.concat("-");
  //       frameBuffer.concat(toNearest(10, (currentPulseEndedAtMicros - previousPulseEndedAtMicros) - pulseLengthMicros));
  //     }
  //     frameBuffer.concat("\t");
  //   }

  //   previousPulseEndedAtMicros = currentPulseEndedAtMicros;
  // }

  /* --- polling --- */

  // String frameBuffer = "";

  // void loop()
  // {
  //   if (digitalRead(REMOTE_DATA_PIN) == LOW) {
  //     wentLowAt = micros();
  //     while (digitalRead(REMOTE_DATA_PIN) == LOW) { }

  //     unsigned int pulseLengthMicros = micros() - wentLowAt;
  //     if (pulseLengthMicros > FRAME_BEGIN_PULSE_MICROS) {
  //       Serial.println(frameBuffer);
  //       frameBuffer = "";

  //       // frameBuffer = "";
  //       // frameBuffer.concat(pulseLengthMicros);
  //       // frameBuffer.concat(" ");
  //     } else {

  //       // // frameBuffer.concat(pulseLengthMicros);
  //       // // frameBuffer.concat("-");
  //       // if (pulseLengthMicros > 170) {
  //       //   frameBuffer.concat(0);
  //       // } else {
  //       //   frameBuffer.concat(1);
  //       // }
  //     }

  //     frameBuffer.concat(pulseLengthMicros);
  //     frameBuffer.concat(" ");
  //   }
  // }

  /* --- */

  // #define FB_LENGTH 128
  // unsigned short frameBuffer[FB_LENGTH];
  // uint8_t fbIndex = 0;

  // void loop() {
  //   if (digitalRead(REMOTE_DATA_PIN) == LOW) {
  //     wentLowAt = micros();
  //     while (digitalRead(REMOTE_DATA_PIN) == LOW) { }

  //     unsigned int pulseLengthMicros = micros() - wentLowAt;
  //     if (pulseLengthMicros > FRAME_BEGIN_PULSE_MICROS) {
  //       fbIndex = 0;

  //       Serial.println("");
  //       for (uint8_t i = 0; i < FB_LENGTH; i++) {
  //         if (frameBuffer[i] > 0) {
  //           Serial.print(frameBuffer[i]);
  //           Serial.print(" ");
  //         }
  //       }
  //     }

  //     frameBuffer[fbIndex] = pulseLengthMicros;
  //     fbIndex++;
  //     if (fbIndex >= FB_LENGTH)
  //       Serial.println("FRAMEBUFFER INDEX OUT OF BOUNDS!");
  //   }
  // }

  // unsigned long lastFrameAtMicros = 0;
  // void loop() {
  //   if (wentLowAt > 0) {
  //     if (digitalRead(REMOTE_DATA_PIN) == HIGH) {
  //       wentHighAt = micros();
  //       digitalWrite(LED_BUILTIN, LOW);

  //       unsigned int lowForMicros = wentHighAt - wentLowAt;

  //       if (lowForMicros > FRAME_BEGIN_PULSE_MICROS) {
  //         // digitalWrite(LED_BUILTIN, HIGH);
  //         // Serial.println(lowForMicros);

  //         unsigned int startedWaiting = micros();

  //         while (digitalRead(REMOTE_DATA_PIN) == HIGH) { }
  //         unsigned int highForMicros = micros() - startedWaiting;

  //         if (highForMicros > FRAME_BEGIN_PULSE_MICROS) {
  //           Serial.println("");
  //           Serial.print(lowForMicros);
  //           Serial.print("-");
  //           Serial.print(highForMicros);
  //           Serial.print("(");
  //           Serial.print((micros() - lastFrameAtMicros)/1000);
  //           Serial.print(")");
  //           lastFrameAtMicros = wentLowAt;
  //         }
  //       } else {
  //         Serial.print(".");
  //       }
  //       wentLowAt = 0;
  //     }
  //   } else {
  //     if (digitalRead(REMOTE_DATA_PIN) == LOW) {
  //       wentLowAt = micros();
  //       wentHighAt = 0;
  //       digitalWrite(LED_BUILTIN, HIGH);
  //     }
  //   }

  //   // digitalWrite(LED_BUILTIN, LOW);
  // }
