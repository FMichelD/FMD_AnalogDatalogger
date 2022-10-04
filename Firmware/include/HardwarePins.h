#ifndef HARDWARE_PINS_H
#define HARDWARE_PINS_H

#include <Arduino.h>

class HardwarePins
{
public:
  // MCP3008 pins
  const uint8_t CS_MCP3008 = 48;

  // SDCardPins
  const uint8_t CS_SDCARD = 44; 
  const uint8_t noSDCardPin = 49;
  const uint8_t noCardLed = 47;
  const uint8_t failCardLed = 46;
  const uint8_t ss = 53;
  const uint8_t cardReset = 6;

  // general pins
  const uint8_t hardwareOkLed = 45;

  // Analog pins
  uint8_t arduinoAnalogInputs[16] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15}; 
  uint8_t numAnalogInputs = 16;

  // DHT22 pins and defines
  const uint8_t DHTPIN = 7;
  #define DHTTYPE DHT22 
};

#endif // HARDWARE_PINS_H
