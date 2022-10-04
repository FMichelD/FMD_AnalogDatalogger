#ifndef DHT22_H
#define DHT22_H

#include <DHT.h>
#include "HardwarePins.h"
   

class DHTSensor: public DHT
{
private:
  HardwareSerial &serial;
  HardwarePins &hardPins;

public:
  DHTSensor(HardwarePins &hardPins, HardwareSerial &serial);
  ~DHTSensor();

  void setup();
};

DHTSensor::DHTSensor(HardwarePins &hardPins, HardwareSerial &serial)
  : hardPins{hardPins}, serial{serial}, DHT(hardPins.DHTPIN, DHTTYPE)
{
}

DHTSensor::~DHTSensor()
{
}

void DHTSensor::setup() 
{
  serial.println("Inicializando DHT22\n");
  begin();
  serial.print("    * DHT22 inicializado: Umidade = ");
  serial.print(readHumidity());
  serial.print("% Temperatura ambiente = ");
  serial.print(readTemperature());
  serial.println("ºC\n");
}

#endif // DHT22_H

