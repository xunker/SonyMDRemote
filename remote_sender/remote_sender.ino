/*

Send commands to a Sony Minidisc Remote Control LCD Display
Matthew Nielsen (C) 2020
https://github.com/xunker/SonyMDRemote

This will "play back" a series of pulses that were captured from a device with
a TRS+4-pin Sony remote jack, to the LCD of another remote control device.

This is *NOT* for sending transport control messages (play, pause, etc).

This code targets the Arduino Due because of its clock speed, but may work with
other Arduino-compatible devices as long as they have a `delaymicroseconds()`
function that is accurate to at least 5 microseconds.

Please be aware of the voltage levels your remote control. can tolerate. Some
are 2v and some may be around 3v, but I can *practically guarantee* that sending
5v in to your remote will kill it. Even sending 3.3v to something with a Vcc of
2.1v could be trouble, so take all appropriate precautions.
*/

/*
This is the pin that will transmit the data to the remote control. If your
remote control uses VCC lower than the Due 3.3v, consider connecting this
through a series of 1n4148 diodes to drop the voltage first.
*/
#define REMOTE_DATA_PIN A0

/*
`commandBuffer[]` is  a list of high/low signals to sent to the remote control.

You can copy/paste output from `remote_dumper.ino` in to this array if you want,
or can uncomment one of the #include's below that to send canned/capture data */

// const signed int commandBuffer[] = {}
// #include "commands_fulldump.h"
#include "commands_track_battery.h"
// #include "commands_volume.h"

// https://github.com/NicksonYap/digitalWriteFast
// FOR ARDUINO DUE, USE: https://github.com/lobbesnl/digitalWriteFast
#include <digitalWriteFast.h>

void setup(){
  pinModeFast(REMOTE_DATA_PIN, OUTPUT);
  digitalWriteFast(REMOTE_DATA_PIN, HIGH);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  Serial.println("Ready");
}

/*
Sends the data contained in the constant `commandBuffer`.
prints the current message number every 10 messages to help decode commands.
*/
void sendCommandBuffer() {
  unsigned int commandLength = sizeof(commandBuffer);
  unsigned int currentLine = 0;
  for (unsigned int i = 0; i < commandLength; i++) {

    if ((currentLine % 10) == 0)
      Serial.println(currentLine);

    /* if the current signal is -, write the pin low else write it high */
    if (commandBuffer[i] < 0) {
      digitalWriteFast(REMOTE_DATA_PIN, LOW);
    } else {
      digitalWriteFast(REMOTE_DATA_PIN, HIGH);
    }

    unsigned short signalLength = abs(commandBuffer[i]);
    /* Below commented out because it may have been causing more problems than it solved */
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

    // If the signal sent is a sync/reset pulse, consider this the start of a new command message
    if ((commandBuffer[i] > -1100) && (commandBuffer[i] < -900)) {
      currentLine++;
    }
  }
}

void loop() {
  sendCommandBuffer();

  digitalWriteFast(REMOTE_DATA_PIN, HIGH);
  Serial.print(micros());
  Serial.println(" Looping");
  delay(1000);
}
