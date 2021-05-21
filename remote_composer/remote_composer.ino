/*

Send commands to a Sony Minidisc Remote Control LCD Display
Matthew Nielsen (C) 2020
https://github.com/xunker/SonyMDRemote

This is for building ("composing") commenads freehand that will be sent to the
remove control LCD of another remote control device.

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

// https://github.com/NicksonYap/digitalWriteFast
// FOR ARDUINO DUE, USE: https://github.com/lobbesnl/digitalWriteFast
#include <digitalWriteFast.h>

void setup() {
  pinModeFast(REMOTE_DATA_PIN, OUTPUT);
  digitalWriteFast(REMOTE_DATA_PIN, HIGH);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  Serial.println("Ready");
}

void sendZero() {
  // Serial.print("0");
  digitalWriteFast(REMOTE_DATA_PIN, LOW);
  delayMicroseconds(220);
  digitalWriteFast(REMOTE_DATA_PIN, HIGH);
  delayMicroseconds(18);
}

void sendOne() {
  // Serial.print("1");
  digitalWriteFast(REMOTE_DATA_PIN, HIGH);
  delayMicroseconds(220);
  digitalWriteFast(REMOTE_DATA_PIN, LOW);
  delayMicroseconds(18);
  digitalWriteFast(REMOTE_DATA_PIN, HIGH);
  delayMicroseconds(18);
}

void sendIdleMessage() {
  digitalWriteFast(REMOTE_DATA_PIN, LOW);
  delayMicroseconds(1067);
  digitalWriteFast(REMOTE_DATA_PIN, HIGH);
  delay(32537 / 1000);
}

void sendSyncReset() {
  digitalWriteFast(REMOTE_DATA_PIN, LOW);
  delayMicroseconds(1105);
  digitalWriteFast(REMOTE_DATA_PIN, HIGH);
  delayMicroseconds(1018);
}

void sendStartBit() { sendZero(); }

/* send the byte with the bit order the remote expects */
void sendByteNatural(byte bin) {
  for (int8_t i = 7; i >= 0; i--) {
    // for (uint8_t i = 0; i < 8; i++) {
    if (bitRead(bin, i)) {
      sendOne();
    } else {
      sendZero();
    }
  }
  // Serial.println("");
}

void send105BitHeader() {
  sendSyncReset();
  sendStartBit();
  sendByteNatural(0b00000000);
  sendByteNatural(0b00000001);
}

void send105BitMessageBody(byte bytes[10]) {
  byte parity = 0b00000000;
  for (uint8_t i = 0; i < 10; i++) {
    sendByteNatural(bytes[i]);
    parity = parity ^ bytes[i];
  }

  sendByteNatural(parity);
}

// https://forum.arduino.cc/index.php?topic=117966.msg887926#msg887926
byte flipByte(byte c) {
  c = ((c >> 1) & 0x55) | ((c << 1) & 0xAA);
  c = ((c >> 2) & 0x33) | ((c << 2) & 0xCC);
  c = (c >> 4) | (c << 4);

  return c;
}

void sendVolumeMessage(uint8_t volume) {
  byte bodyBytes[10] = {0b00000010,        // Volume Message 0x40
                        flipByte(volume),  // 0b00110000,
                        0b00000010,
                        0b01111001,
                        0b00000000,
                        0b00000000,
                        0b00000000,
                        0b00000000,
                        0b00000000,
                        0b00000000};
  send105BitHeader();
  send105BitMessageBody(bodyBytes);
}

void sendTrackNumberMessage(uint8_t track) {
  Serial.print(track);
  Serial.print(" ");

  uint8_t hundreds = 0;
  if (track >= 100) {
    hundreds = track / 100;
    track = track - (hundreds * 100);
  }

  uint8_t tens = 0;
  if (track >= 10) {
    tens = (track / 10);
    track = track - (tens * 10);
  }

  uint8_t ones = track;

  byte bodyBytes[10] = {0b00000101,  // Track number message  0xA0
                        0b00000000,
                        0b00000000,
                        flipByte(hundreds),            // hundreds
                        flipByte((tens << 4) ^ ones),  // tens/ones
                        0b00000000,
                        0b00000000,
                        0b00000000,
                        0b00000000,
                        0b00000000};
  send105BitHeader();
  send105BitMessageBody(bodyBytes);
}

const byte batteryLevels[] = {
    0x80,  // LoBATT
    0x01,  // 0/4 (1 bar Blinking)
    0x9f,  // 1/4 Bars
    0xbf,  // 2/4 Bars
    0xdf,  // 3/4 Bars
    0xff,  // 4/4 Bars
    0x7f   // Charge
};

void sendBatteryLevelMessage(uint8_t level) {
  byte bodyBytes[10] = {0b11000010,  // Battery Level Message 0x43
                        flipByte(batteryLevels[level]),
                        0b00000000,
                        0b00000000,
                        0b00000000,
                        0b00000000,
                        0b00000000,
                        0b00000000,
                        0b00000000,
                        0b00000000};
  send105BitHeader();
  send105BitMessageBody(bodyBytes);
}

byte vol = 20;
signed short volDir = 1;

void volumeLoop() {
  sendIdleMessage();
  sendIdleMessage();
  sendIdleMessage();
  sendVolumeMessage(vol);

  vol += volDir;
  if (vol >= 30) volDir = -1;
  if (vol <= 0) volDir = +1;
}

uint8_t trackNumber = 90;
signed short trackNumberDir = 1;

void trackNumberLoop() {
  sendIdleMessage();
  sendIdleMessage();
  sendIdleMessage();
  sendTrackNumberMessage(trackNumber);

  trackNumber += trackNumberDir;
  if (trackNumber >= 99) trackNumberDir = -1;
  if (trackNumber <= 0) trackNumberDir = +1;
}

byte batteryLevel = 0;
signed short batteryLevelDir = 1;

void batteryLevelLoop() {
  sendIdleMessage();
  sendIdleMessage();
  sendIdleMessage();
  sendBatteryLevelMessage(batteryLevel);

  batteryLevel += batteryLevelDir;
  if (batteryLevel >= sizeof(batteryLevels)) batteryLevelDir = -1;
  if (batteryLevel <= 0) batteryLevelDir = +1;
}

void loop() {
  volumeLoop();
  trackNumberLoop();
  batteryLevelLoop();

  digitalWriteFast(REMOTE_DATA_PIN, HIGH);
  Serial.print(micros());
  Serial.println(" Looping");
  delay(1000);
}
