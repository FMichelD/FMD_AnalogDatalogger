#ifndef RTCTIMER_H
#define RTCTIMER_H

#include <Wire.h>
#include <RTClib.h>
#include <HardwareSerial.h>

class RtcTimer: public DS1307
{
private:
  char dataAndTime[20];
  HardwarePins &hardPins;
  HardwareSerial &serial;

public:
  RtcTimer(HardwarePins &hardPins, HardwareSerial &serial);
  ~RtcTimer();
  
  void setup();
  const char* getDateTime();
};

RtcTimer::RtcTimer(HardwarePins &hardPins, HardwareSerial &serial)
   : hardPins{hardPins}, serial{serial}
{
}

RtcTimer::~RtcTimer()
{
}

void RtcTimer::setup() {

  serial.println("Inicializando relógio (RTC)");
  Wire.begin();
  begin();

  if (!isrunning()) {
    serial.println("    ! RTC is NOT running - -check batery");
    // following line sets the RTC to the date & time this sketch was compiled
    // adjust(DateTime(__DATE__, __TIME__));
  }
  // adjust(DateTime(__DATE__, __TIME__));
  serial.print("    * RTC configurado: ");
  serial.println(now().tostr(dataAndTime));
}

const char*  RtcTimer::getDateTime()
{
  now().tostr(dataAndTime);
  // serial.println(dataAndTime);
  return dataAndTime;
}

#endif // RTCTIMER_H


