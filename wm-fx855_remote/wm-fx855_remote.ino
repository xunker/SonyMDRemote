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

#ifndef ESP32
#error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif

/*
This is the pin that will receiev the data from the player. If your device
uses "active-low" to send data I recommend you connect a 100k-470k resistor
between this pin and ground.
*/
#define REMOTE_DATA_PIN 12

// https://github.com/khoih-prog/ESP32TimerInterrupt
#include "ESP32TimerInterrupt.h"
ESP32Timer ITimer0(0);

// Some hints for heltec wifi lora
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
// Define proper RST_PIN if required.
#define RST_PIN 16
SSD1306AsciiWire oled;

#ifndef LED_BUILTIN
#define LED_BUILTIN 25
#endif

volatile unsigned long currentLevelStartedAt = 0;
volatile unsigned long previousLevelStartedAt = 0;
volatile boolean breakNow = false;

/* any signal, high or low, that is longer than this will be treated as the end of one message and the beginning of another */
#define END_OF_MESSAGE_TIMEOUT_MICROSECONDS 10000

// whenever the player is not sending a signal, delay this long each loop to avoid needlessly checking the same bool
#define NO_SENDER_LOOP_DELAY_MICROSECONDS 500

#define START_HIGH_MICROSECONDS 4900-NO_SENDER_LOOP_DELAY_MICROSECONDS // about 5ms, but add buffer for the interrupt to latch
#define START_LOW_MICROSECONDS 1900 // about 2ms

uint32_t currentCommandBytes = 0; // expected to contain 3 bytes
volatile boolean inMessage = false;

#define PLAYING_FWD 0
#define PLAYING_REV 1
boolean tapeDirection = PLAYING_FWD;

#define ACTION_OFF 0
#define ACTION_PLAYING 1
#define ACTION_REW 2
#define ACTION_FF 3
#define ACTION_STOP 4
uint8_t currentAction = ACTION_OFF;

#define TAPE_MODE 0
#define RADIO_MODE 1
boolean functionMode = TAPE_MODE;

#define RADIO_AM 0b00000000
#define RADIO_FM 0b00000010
#define RADIO_TV 0b00000011
uint8_t radioBand = RADIO_AM;
uint16_t frequency = 0;

uint8_t currentPreset = 0;

void updateDisplay() {
  oled.clear();

  oled.setCursor(0, 0);
  if (functionMode == RADIO_MODE) {
    oled.print("RADIO");

    oled.setCursor(oled.displayWidth() - oled.fieldWidth(5), 0);
    if (currentPreset > 0) {
      oled.print("PRESET ");
      oled.print(currentPreset);
    }

    oled.setCursor(0, 2);
    if (radioBand == RADIO_AM) {
      oled.print(String(frequency));
      oled.print(" kHz AM");
    } else if (radioBand == RADIO_FM) {
      oled.print(String((float)frequency / 10));
      oled.print(" mHz FM");
    } else if (radioBand == RADIO_TV) {
      oled.print("TV Channel ");
      oled.print(String(frequency));
    } else {
      oled.print(String(frequency));
      oled.print(" huh?");
    }

  } else {
    oled.print("TAPE");

    oled.setCursor(0, 2);
    if (currentAction == ACTION_PLAYING) {
      oled.print("PLAY");
    } else if (currentAction == ACTION_REW) {
      oled.print("REW");
    } else if (currentAction == ACTION_FF) {
      oled.print("FF");
    } else if (currentAction == ACTION_STOP) {
      oled.print("STOP");
    } else {
      oled.print("????");
    }
  }

  oled.setCursor(oled.displayWidth() - oled.fieldWidth(3), 5);
  if (tapeDirection == PLAYING_FWD) {
    oled.print("FWD");
  } else {
    oled.print("REV");
  }
}

void IRAM_ATTR TimerHandler0(void) {
  // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  if (!inMessage)
    return;

  if (currentLevelStartedAt == previousLevelStartedAt)
    breakNow = true;

  previousLevelStartedAt = currentLevelStartedAt;
}

volatile boolean senderIsTransmitting = false;
// volatile unsigned long lastMessageReceivedAtMicroseconds = 0;

void IRAM_ATTR waitForHighISR() {
  detachInterrupt(REMOTE_DATA_PIN);
  senderIsTransmitting = true;
}

void attachWaitForHighInterrupt(){
  attachInterrupt(REMOTE_DATA_PIN, waitForHighISR, RISING);
}

void setup() {
  Wire.begin(SDA_OLED, SCL_OLED);
  Wire.setClock(400000L);

  #if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // RST_PIN >= 0

  oled.displayRemap(true); // flip display
  // oled.setFont(System5x7);
  // oled.setFont(font8x8);
  oled.setFont(Verdana12);
  oled.setScrollMode(SCROLL_MODE_AUTO);
  oled.clear();
  oled.setCursor(0,0);
  oled.print("Hello world!");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  Serial.println("Ready");

  if (ITimer0.attachInterruptInterval(100 * 1000, TimerHandler0)) // call every 100ms
    Serial.println("Starting  ITimer0 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer0. Select another freq. or timer");

  attachWaitForHighInterrupt();
}

// https://forum.arduino.cc/index.php?topic=42465.0
uint8_t getBitsFromCommandByte(uint8_t b, int offset, int count) {
  return (b >> offset) & ((1 << count) - 1);
}

uint8_t getBitsFromAttributeBytes(uint16_t b, int offset, int count) {
  return (b >> offset) & ((1 << count) - 1);
}

uint8_t getCommandByte(uint32_t payload) {
  if (payload > 65535) {
    return (payload >> 16) & ((1 << 8) - 1);
  } else {
    return (payload >> 0) & ((1 << 8) - 1);
  }
}

uint16_t getAttributeBytes(uint32_t payload) {
  if (payload > 255) {
    return (payload >> 0) & ((1 << 16) - 1);
  }
  return 0;
}

unsigned long waitForLevelAndRecord(boolean level) {
  currentLevelStartedAt = micros();

  while (digitalRead(REMOTE_DATA_PIN) == level) {
    if (breakNow)
      break;
  }

  unsigned long currentLevelEndedAt = micros();
  unsigned long currentLevelLength = currentLevelEndedAt - currentLevelStartedAt;

  if (breakNow)
    breakNow = false;

  previousLevelStartedAt = 0;

  return currentLevelLength;
}

void processPayload() {
  uint8_t commandByte = getCommandByte(currentCommandBytes);
  uint16_t attributeBytes = getAttributeBytes(currentCommandBytes);

  // Serial.println(currentCommandBytes, BIN);
  if (attributeBytes > 0) {
    Serial.print(commandByte, BIN);
    Serial.print(" ");
    Serial.println(attributeBytes, BIN);
  } else {
    Serial.println(commandByte, BIN);
  }

  if ((getBitsFromCommandByte(commandByte, 6, 1) == 0b00000001) && (attributeBytes > 0)) {
    functionMode = RADIO_MODE;

    currentPreset = getBitsFromCommandByte(commandByte, 0, 5);
    radioBand = getBitsFromAttributeBytes(attributeBytes, 0, 2);
    if (radioBand == RADIO_AM) {
      // check if it's really TV mode
      if (getBitsFromAttributeBytes(attributeBytes, 6, 2) == 0b00000011) {
        radioBand = RADIO_TV;
      }
    }
    frequency = 0;

    if (radioBand == RADIO_AM) {
      uint8_t ones = getBitsFromAttributeBytes(attributeBytes, 4, 4);
      frequency = frequency + ones;

      uint8_t tens = getBitsFromAttributeBytes(attributeBytes, 8, 4);
      frequency = frequency + (10 * tens);

      uint8_t hundreds = getBitsFromAttributeBytes(attributeBytes, 12, 4);
      frequency = frequency + (100 * hundreds);

      uint8_t thousands = getBitsFromAttributeBytes(attributeBytes, 2, 2);
      frequency = frequency + (1000 * thousands);
    } else if (radioBand == RADIO_FM) {
      uint8_t ones = getBitsFromAttributeBytes(attributeBytes, 8, 4);
      frequency = frequency + (10 * ones);

      uint8_t tens = getBitsFromAttributeBytes(attributeBytes, 12, 4);
      frequency = frequency + (100 * tens);

      uint8_t hundreds = getBitsFromAttributeBytes(attributeBytes, 2, 2);
      frequency = frequency + (1000 * hundreds);

      uint8_t decimal = getBitsFromAttributeBytes(attributeBytes, 4, 4);
      frequency = frequency + decimal;
    } else if (radioBand == RADIO_TV) {
      uint8_t ones = getBitsFromAttributeBytes(attributeBytes, 12, 4);
      frequency = frequency + (1 * ones);

      uint8_t tens = getBitsFromAttributeBytes(attributeBytes, 2, 2);
      frequency = frequency + (10 * tens);
    }

    if (getBitsFromCommandByte(commandByte, 5, 1) == 0b00000000) {
      tapeDirection = PLAYING_FWD;
    } else if (getBitsFromCommandByte(commandByte, 5, 1) == 0b00000001) {
      tapeDirection = PLAYING_REV;
    }
  } else if (getBitsFromCommandByte(commandByte, 6, 1) == 0b00000000) {
    functionMode = TAPE_MODE;

    if (getBitsFromCommandByte(commandByte, 4, 1) == 0b00000000) {
      tapeDirection = PLAYING_FWD;
    } else if (getBitsFromCommandByte(commandByte, 4, 1) == 0b00000001) {
      tapeDirection = PLAYING_REV;
    }

    if (getBitsFromCommandByte(commandByte, 3, 1) == 0b00000001) {
      currentAction = ACTION_PLAYING;
    } else if (getBitsFromCommandByte(commandByte, 5, 1) == 0b00000001) {
      currentAction = ACTION_FF;
    } else if (getBitsFromCommandByte(commandByte, 5, 2) == 0b00000011) {
      currentAction = ACTION_REW;
    } else if (getBitsFromCommandByte(commandByte, 2, 1) == 0b00000001) {
      currentAction = ACTION_STOP;
    }
  }

  updateDisplay();
}

void loop() {
  if (senderIsTransmitting) {
    digitalWrite(LED_BUILTIN, HIGH);
    if (inMessage) {
      unsigned long highPeriod = waitForLevelAndRecord(HIGH);
      unsigned long lowPeriod = waitForLevelAndRecord(LOW);

      if (lowPeriod >= END_OF_MESSAGE_TIMEOUT_MICROSECONDS) {
        inMessage = false;
        return;
      }

      byte currentBit = 0;
      if ((highPeriod >= 1900) && (lowPeriod >= 550)) {
        currentBit = 0b00000001;
      } else if ((highPeriod >= 550) && (lowPeriod >= 1900)) {
        currentBit = 0b00000000;
      } else {
        // some kind of error, try again later
        inMessage = false;
        return;
      }
      currentCommandBytes = currentCommandBytes ^ currentBit;
      currentCommandBytes = currentCommandBytes << 1;
    } else {
      if (currentCommandBytes > 0) {
        // not in a message but there are bytes to be processed, so that usually
        // means we've received the whole message and now need to process it
        processPayload();
        currentCommandBytes = 0;

        senderIsTransmitting = false;
        attachWaitForHighInterrupt();

      }

      if (waitForLevelAndRecord(HIGH) >= START_HIGH_MICROSECONDS) {
        if (waitForLevelAndRecord(LOW) >= START_LOW_MICROSECONDS) {
          inMessage = true;
          currentCommandBytes = 0;
        }
      }
    }
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    delayMicroseconds(NO_SENDER_LOOP_DELAY_MICROSECONDS);
  }
}