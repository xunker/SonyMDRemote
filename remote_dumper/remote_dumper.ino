/*

Receive and dump LCD display commands from a Sony Minidisc Remote Control port
Matthew Nielsen (C) 2020
https://github.com/xunker/SonyMDRemote

This will record the LCD control signals being sent from the remote control data
pin of a Sony Minidisc remote control port (TRS+4-pin jack), and will dump that
data to the serial console as a series of individual messages.

This is *NOT* for reading transport control messages (play, pause, etc).

This is targeted at the Arduino Due for its high clock speed. Other
microcontrollers may work if they can read from a pin at ~1 microsecond
intervals.

This sketch continually polls REMOTE_DATA_PIN and records when it changes from
HIGH to LOW. It watches for the signal that marks the beginning of a new message
and groups high/low signals by those markers. When a complete message is
received, it is sent to the serial console on a single line for easy parsing.

The data sent to the serial console is formatted so it can be easily
copy/paste'd in to the complimentary `remote_sender.ino` sketch.

For player devices that will pull the data line high, a high-value resistor
between the data line and the signal ground is recommended for clear readings.
I have used 100k to 470k with success.

If your player device uses VCC of less than 3 volts (devices made around 2004 or
later generally), you may have trouble reading the high/low signals with a 5v
microcontroller. I recommend a using 3.3v microcontroller.

*/

/*
This is the pin that will receiev the data from the player. If your device
uses "active-low" to send data I recommend you connect a 100k-470k resistor
between this pin and ground.
*/
#define REMOTE_DATA_PIN A1

// https://github.com/NicksonYap/digitalWriteFast
// FOR ARDUINO DUE, USE: https://github.com/lobbesnl/digitalWriteFast
#include <digitalWriteFast.h>

void setup() {
  pinModeFast(REMOTE_DATA_PIN, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  Serial.println("Ready");
}

unsigned long wentLowAt = 0;
unsigned long wentHighAt = 0;

/* the length of the low signal that marks the beginning of a new message */
#define FRAME_BEGIN_PULSE_MICROS 800

String frameBuffer = "";

void loop()
{
  unsigned long currentLowStartedAt = micros();
  while (digitalReadFast(REMOTE_DATA_PIN) == LOW) {}

  unsigned long currentLowEndedAt = micros();

  unsigned long currentLowLength = currentLowEndedAt - currentLowStartedAt;
  if (currentLowLength > FRAME_BEGIN_PULSE_MICROS) {
    Serial.println(frameBuffer);
    frameBuffer = "";
  }

  frameBuffer.concat("-");
  frameBuffer.concat(currentLowLength);
  frameBuffer.concat(", ");

  unsigned long currentHighStartedAt = micros();
  while (digitalReadFast(REMOTE_DATA_PIN) == HIGH) { }

  unsigned long currentHighEndedAt = micros();

  unsigned long currentHighLength = currentHighEndedAt - currentHighStartedAt;
  frameBuffer.concat("+");
  frameBuffer.concat(currentHighLength);
  frameBuffer.concat(", ");
}
