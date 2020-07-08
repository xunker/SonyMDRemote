/*

Remote control for Sony WM-FX855 Cassette Walkman
Matthew Nielsen (C) 2020
https://github.com/xunker/SonyMDRemote

Made to replace the OEM remote control for the WM-FX755 Walkman. Provides
feedback on OLED display and can send remote control commands.

Based on the Heltec ESP32-based "WiFi Kit 32" with built-in OLED display, but
any ESP32 with a connected OLED should work.

Signals from walkman must be boosted from their 1.4v logic level to something
the ESP32 can read using a comparator circuit.

Sending commands to the Walkman done using MCP4251 digital potentiometer.

Uses:
* https://github.com/khoih-prog/ESP32TimerInterrupt
* SSD1306 arduino library
* https://forum.arduino.cc/index.php?topic=42465.0
* https://arduino.stackexchange.com/questions/55805/spi-digital-potentiometer-not-working-on-esp32
*/

#ifndef ESP32
#error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif

/*
This is the pin that will receive the data from the player. Will need to be
boosted by a comparator circuit or else the ESP32 won't be able to "see" it.
*/
#define REMOTE_DATA_PIN 27

// outside in
const uint8_t buttonPins[] = {
  // 36,
  //skipped 37,
  // 38,
  // skipped 39,
  // 34,
  // skipped 35,
  // 32,
  // skipped 33,
  // 25, // collides with builtin led
  // 26,
  // skipped 14,
  12,
  // skipped 13, RST, 3v3, 5v, gnd
};
#define PIN_BAND buttonPins[0]

volatile uint8_t remoteCommandPending = 0;
// number is the digipot divisions to send
#define REMOTE_COMMAND_BAND 10
// #define REMOTE_COMMAND_PLAY 2
// #define REMOTE_COMMAND_STOP 3
// #define REMOTE_COMMAND_REV 4
// #define REMOTE_COMMAND_FWD 5
// #define REMOTE_COMMAND_MEGA_BASS 6

#define REMOTE_COMMAND_BAND_LABEL 0 // index to commandLabels
#define REMOTE_COMMAND_PLAY_LABEL 1
#define REMOTE_COMMAND_STOP_LABEL 2
#define REMOTE_COMMAND_REV_LABEL 3
#define REMOTE_COMMAND_FWD_LABEL 4
#define REMOTE_COMMAND_MEGA_BASS_LABEL 5

// pointer to label in list
volatile uint8_t remoteCommandPendingLabel = 0;
const String commandLabels[] = {
  "band",
  "play",
  "stop",
  "prev",
  "next",
  "bass"
};

// blank the screen after xx seconds to preserve power and prevent wear
#define ENABLE_SCREEN_BLANK

#define ENABLE_SLEEP
// to enable sleep, you will also need to separately set SLEEP_WAKEUP_PIN to
// be the same pin as REMOTE_DATA_PIN using the GPIO_NUM_* macro
#define SLEEP_WAKEUP_PIN GPIO_NUM_27 // RTC_GPIO17

// turn the CPU down to 80mhz (from default 240) to save power
#define LOW_CPU_SPEED

// https://github.com/khoih-prog/ESP32TimerInterrupt
#include "ESP32TimerInterrupt.h"
ESP32Timer ITimer0(0);

// Some hints for heltec wifi lora
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#include "esp32-hal-cpu.h"

#define MCP4251_SHUTDOWN_PIN 13
#define MCP4251_CS_PIN 5 // 5 VSPI, 15 HSPI
#include "mcp4251.h"

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

unsigned long lastMessageReceivedAtMillis = 0;
#define SLEEP_DISPLAY_AFTER_INACTIVITY_MILLISECONDS 10000

/* any signal, high or low, that is longer than this will be treated as the end of one message and the beginning of another */
#define END_OF_MESSAGE_TIMEOUT_MICROSECONDS 10000

// whenever the player is not sending a signal, delay this long each loop to avoid needlessly checking the same bool
// only applicable if ENABLE_SLEEP is *not* enabled
#define NO_SENDER_LOOP_DELAY_MICROSECONDS 100

// about 5ms, but add buffer for the interrupt to latch and to accoutn for waking from sleep
#define START_HIGH_MICROSECONDS 4900-(NO_SENDER_LOOP_DELAY_MICROSECONDS+500)
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
#define UNKNOWN_MODE 99
boolean functionMode = UNKNOWN_MODE;

#define RADIO_AM 0b00000000
#define RADIO_FM 0b00000010
#define RADIO_TV 0b00000011
uint8_t radioBand = RADIO_AM;
uint16_t frequency = 0;

uint8_t currentPreset = 0;

uint8_t currentAmsIndex = 0;

uint8_t currentBassBoost = 0;

// indicates display need to be redrawn
volatile boolean displayDirty = false;
// indicates remote command portion of display only need to be redrawn
volatile boolean displayRemoteCommandDirty = false;

// show current remote control command being executed on screen
void updateCurrentCommand() {
  if (remoteCommandPending > 0) {
    oled.setCursor(0, 5);
    oled.print(commandLabels[remoteCommandPendingLabel]);
  } else {
    oled.clearField(0, 5, 5);
  }

}

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

  } else if (functionMode == TAPE_MODE) {
    oled.print("TAPE");

    oled.setCursor(0, 2);
    switch (currentAction){
      case ACTION_PLAYING:
        oled.print("PLAY");
        break;
      case ACTION_REW:
        oled.print("REW");
        break;
      case ACTION_FF:
        oled.print("FF");
        break;
      case ACTION_STOP:
        oled.print("STOP");
        break;
      default:
        oled.print("????");
        break;
    }

    if (currentAmsIndex > 0) {
      oled.print(" AMS ");
      oled.print(currentAmsIndex + 1);
    }
  } else {
    oled.print("UNKNOWN MODE");
  }

  oled.setCursor(oled.displayWidth() - oled.fieldWidth(3), 5);
  if (tapeDirection == PLAYING_FWD) {
    oled.print("FWD");
  } else {
    oled.print("REV");
  }

  updateCurrentCommand();
}

volatile boolean senderIsTransmitting = false;

void IRAM_ATTR TimerHandler0(void) {
  // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  if (!inMessage) {
    if ((senderIsTransmitting) && (currentLevelStartedAt == previousLevelStartedAt)) {
      // indicates we're polling needlessly
      senderIsTransmitting = false;
      previousLevelStartedAt = currentLevelStartedAt;
    }
    return;
  }

  if (currentLevelStartedAt == previousLevelStartedAt)
    breakNow = true;

  previousLevelStartedAt = currentLevelStartedAt;
}

void IRAM_ATTR waitForHighISR() {
  detachInterrupt(REMOTE_DATA_PIN);
  senderIsTransmitting = true;
}

void attachWaitForHighInterrupt(){
  attachInterrupt(REMOTE_DATA_PIN, waitForHighISR, RISING);
}

void sendRemoteCommand(uint8_t potPos) {
  Serial.println("Sending " + String(potPos, DEC));
  digitalPotWrite(potPos);
  wakeDigitalPot();
  delay(500);
  shutdownDigitalPot();
}

// to be called from xTaskCreate
void sendRemoteCommandPending(void * parameter) {

  sendRemoteCommand(remoteCommandPending);
  remoteCommandPending = 0;
  displayRemoteCommandDirty = true;
  vTaskDelete(NULL);
}

void sendScheduledButton() {
  if (remoteCommandPending == 0)
    return;

  displayRemoteCommandDirty = true;

  xTaskCreate(
    sendRemoteCommandPending,
    "sendRemoteCommandPending",
    10000, //stack size
    NULL, // args
    0, // priority
    NULL //task handle
  );
}

void scheduleBandButton() {
  if (remoteCommandPending == REMOTE_COMMAND_BAND)
    return;
  remoteCommandPending = REMOTE_COMMAND_BAND;
  remoteCommandPendingLabel = 0;

  sendScheduledButton();
}

void attachBandButtonInterrupt(uint8_t pin){
  attachInterrupt(pin, scheduleBandButton, RISING);
}

void setup() {
  #ifdef LOW_CPU_SPEED
    setCpuFrequencyMhz(80); //Set CPU clock to 80MHz to save power
  #endif

  Wire.begin(SDA_OLED, SCL_OLED);
  Wire.setClock(400000L);

  #if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // RST_PIN >= 0

  oled.displayRemap(true); // flip display
  oled.setFont(Callibri15);
  oled.setScrollMode(SCROLL_MODE_AUTO);
  oled.clear();
  oled.setCursor(0,0);
  oled.println("WM-FX855 Remote");
  oled.print(getCpuFrequencyMhz()); oled.println("mHz mode");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  Serial.println("Ready");

  if (ITimer0.attachInterruptInterval(100 * 1000, TimerHandler0)) // call every 100ms
    Serial.println("Starting  ITimer0 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer0. Select another freq. or timer");

  mcp4251Setup();


  for (uint8_t i = 0; i < sizeof(buttonPins); i++) {
    pinMode(buttonPins[i], INPUT);
  };
  attachBandButtonInterrupt(PIN_BAND);

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

void reportUnknownMessage(uint8_t commandByte) {
  Serial.println("Unknown message: " + String(commandByte, BIN));
  oled.clear();
  oled.println("Unknown message:");
  oled.println(String(commandByte, BIN));
}

void processRadioMessage(uint8_t commandByte, uint16_t attributeBytes) {
  Serial.println("Radio Message");
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
}

void processTapeMessage(uint8_t commandByte) {
  Serial.println("Tape Message");
  if (getBitsFromCommandByte(commandByte, 4, 1) == 0b00000001) {
    tapeDirection = PLAYING_REV;
  } else {
    tapeDirection = PLAYING_FWD;
  }

  if ((commandByte & 0b01100000) == 0b01100000) {
    // unknown use
    return;
  } else if ((commandByte & 0b00100000) == 0b00100000) {
    currentAction = ACTION_FF;
    // get AMS index from last four
    currentAmsIndex = getBitsFromCommandByte(commandByte, 0, 4);
  } else if ((commandByte & 0b01000000) == 0b01000000) {
    currentAction = ACTION_REW;
    // get AMS index from last four
    currentAmsIndex = getBitsFromCommandByte(commandByte, 0, 4);
  } else {
    currentAmsIndex = 0;
    if ((commandByte & 0b00000100) == 0b00000100) {
      currentAction = ACTION_STOP;
    } else if ((commandByte & 0b00001000) == 0b00001000) {
      currentAction = ACTION_PLAYING;
    }
  }
}

// messages that begin with 011
void processStateMessage(uint8_t commandByte) {
  Serial.println("State Message");
  // todo modal message for bass boost
  currentBassBoost = getBitsFromCommandByte(commandByte, 0, 3);
}

void printFormattedMessage(uint8_t commandByte, uint16_t attributeBytes) {
  char commandBuffer[9];
  char commandBuffer2[9];
  itoa (commandByte,commandBuffer,2);
  sprintf(commandBuffer2, "%08s", commandBuffer);

  if (attributeBytes > 0) {
    char attributesBuffer[17];
    char attributesBuffer2[17];
    itoa (attributeBytes,attributesBuffer,2);
    sprintf(attributesBuffer2, "%016s", attributesBuffer);

    Serial.print(commandBuffer2);
    Serial.print(" ");
    Serial.println(attributesBuffer2);
  } else {
    Serial.println(commandBuffer2);
  }
}

void processPayload() {
  uint8_t commandByte = getCommandByte(currentCommandBytes);
  uint16_t attributeBytes = getAttributeBytes(currentCommandBytes);

  printFormattedMessage(commandByte, attributeBytes);

  /* There's probably a better way to deduce the current mode (radio or tape)
  using the bits in the command byte, but instead we just assume that if there
  are no Attributes bytes are present after the command byte, then we are in
  tape mode. */
  if (attributeBytes > 0) {
    if (getBitsFromCommandByte(commandByte, 6, 2) == 0b00000011) {
      functionMode = RADIO_MODE;
      processRadioMessage(commandByte, attributeBytes);
    } else {
      reportUnknownMessage(commandByte);
      functionMode == UNKNOWN_MODE;
    }
    updateDisplay();
  } else {
    /* "skippable" messages: don't know that they mean, but we've functioned long enough without them! */
    if (getBitsFromCommandByte(commandByte, 5, 3) == 0b00000011) {
      processStateMessage(commandByte);
    } else if (getBitsFromCommandByte(commandByte, 7, 1) == 0b0000000) {
      functionMode = TAPE_MODE;
      processTapeMessage(commandByte);
      updateDisplay();
    } else {
      reportUnknownMessage(commandByte);
    }
  }
}

void indicatePollingModeEnabled() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void indicatePollingModeDisabled() {
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (displayDirty) {
    updateDisplay();
    displayDirty = false;
    displayRemoteCommandDirty = false;
  }

  if (displayRemoteCommandDirty) {
    updateCurrentCommand();
    displayRemoteCommandDirty = false;
  }

  if (senderIsTransmitting) {
    indicatePollingModeEnabled();
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

        lastMessageReceivedAtMillis = millis();

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
    indicatePollingModeDisabled();

    // keep this delay *before* the sleep. We don't want the MCU to wake from sleep and then immediately delay!
    delayMicroseconds(NO_SENDER_LOOP_DELAY_MICROSECONDS);

    unsigned long currentMillis = millis();
    if ((currentMillis > SLEEP_DISPLAY_AFTER_INACTIVITY_MILLISECONDS) && (lastMessageReceivedAtMillis < (currentMillis - SLEEP_DISPLAY_AFTER_INACTIVITY_MILLISECONDS))) {
      lastMessageReceivedAtMillis = currentMillis;
      #ifdef ENABLE_SCREEN_BLANK
        // activity timeout, sleep the display
        oled.clear();
      #endif

      #ifdef ENABLE_SLEEP
        detachInterrupt(REMOTE_DATA_PIN);
        senderIsTransmitting = true;
        // sleep
        // esp_sleep_enable_ext0_wakeup(SLEEP_WAKEUP_PIN, HIGH);
        esp_sleep_enable_ext1_wakeup(0b00001000000000000001000000000000, ESP_EXT1_WAKEUP_ANY_HIGH);
        esp_light_sleep_start();
      #endif
    }

  }
}
