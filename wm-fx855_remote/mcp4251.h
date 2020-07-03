// Based on:
// https://arduino.stackexchange.com/questions/55805/spi-digital-potentiometer-not-working-on-esp32

/*
findings:
1 play
2 play
3 play
5 play
10 ffwd
Pot Pos: 15 rew (momentary?)
01110000
00010100
20 band
25 ?
Pot Pos: 30 # will send this data even while "off"
01100000
Pot Pos: 30
01100100
Pot Pos: 30
01100110
Pot Pos: 30
01100000
Pot Pos: 30
01100100
Pot Pos: 30
01100110
35 01100100
40
45
50
55

*/

#ifndef MCP4251_H
#define MCP4251_H

#ifndef ESP32
#error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif

#include <Arduino.h>
#include <SPI.h>

static const int spiClk = 1000000;
byte address = 0x00;
SPIClass * hspi = NULL;

#ifndef MCP_SHUTDOWN
#define MCP_SHUTDOWN 0x2000
#endif

void shutdownDigitalPot() {
  digitalWrite(MCP4251_SHUTDOWN_PIN, LOW);
}

void wakeDigitalPot() {
  digitalWrite(MCP4251_SHUTDOWN_PIN, HIGH);
}

void mcp4251Setup() {
  pinMode(MCP4251_SHUTDOWN_PIN, OUTPUT);
  shutdownDigitalPot();

  pinMode(MCP4251_CS_PIN, OUTPUT);
  // HSPI pins (sclk: 14, miso: 12, mosi: 13, cs: 15)
  // VSPI pins (sclk: 18, miso: 19, mosi: 23, cs: 5)
  hspi = new SPIClass(VSPI); // or VSPI
  hspi->begin();
}

// void loop() {
//   for (int i = 0; i <= 255; i = i + 15)
//   {
//     digitalPotWrite(i);
//     delay(5000);
//   }

//   delay(5000);
//   for (int i = 255; i >= 0; i = i - 15)
//   {
//     digitalPotWrite(i);
//     delay(5000);
//   }
//   delay(5000);
// }

int digitalPotWrite(int value)
{
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(MCP4251_CS_PIN, LOW);
  hspi->transfer(address);
  hspi->transfer(value);
  digitalWrite(MCP4251_CS_PIN, HIGH);
  hspi->endTransaction();
}

#endif
