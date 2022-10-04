#include "Datalogger.h"
#include "SDCardRW.h"

#include <Arduino.h>
#include <HardwareSerial.h>

Datalogger dl;

void setup() {  
  dl.setupHardware();
  delay(500);

  // // // change the ID and file name for each datalogger
  strcpy(dl.ID, "4");
  strcpy(dl.fileName, "arduino_4.csv");


  dl.card.createFile(dl.fileName);
  dl.readNativeAnalogInputs();
  // dl.card.readWriteTest(dl.fileName);

  dl.lastSave = dl.rtc.now();

  dl.serial.println("<Arduino is ready>");

  // Serial.begin(115200);
  // // // Serial.println("Inicializando...");
  // // // Serial.println("Inicializando relógio (RTC)");
  // // // Serial.println("* RTC configurado: ");  
  // Serial.println("<Arduino is ready>");
}


void testPrint() {
    const char tmp[12];
    dl.serial.print("save;");
    dl.serial.print(dl.ID);
    dl.serial.print(";");
    strncpy(tmp, dl.rtc.getDateTime(), 10); //copy date YYYY/MM/DD from dateTime "YYYY/MM/DD hh:mm:ss" to tmp
    dl.serial.print(tmp);
    // dl.serial.print("");
    dl.serial.print(";");
    strncpy(tmp, "\0\0\0\0\0\0\0\0\0\0\0\0", 12);
    strncpy(tmp, dl.rtc.getDateTime()+11, 9); //copy time hh:mm:ss from dateTime "YYYY/MM/DD hh:mm:ss" to tmp
    dl.serial.print(tmp);
    dl.serial.print(";");
    dl.serial.print("0000000");
    dl.serial.print(";");
    dl.serial.print(dl.dht22.readHumidity());
    dl.serial.print(";");
    dl.serial.println(dl.dht22.readTemperature());
  
}


void loop() {

  while(!dl.card.isCardPresent() && !dl.card.cardInitialized){
    dl.card.initializeCard();
    digitalWrite(dl.hardPins.hardwareOkLed, LOW);

  }
  digitalWrite(dl.hardPins.hardwareOkLed, HIGH);

  dl.rtc.getDateTime();

  if((dl.rtc.now() - dl.lastSave) >= TimeDelta(dl.readIntFromEEPROM(dl.addrRegisterInterval))) {
    dl.readNativeAnalogInputs();
    // dl.serial.println("teste");
    dl.lastSave = dl.rtc.now();
  }
  // delay(1000);

  dl.receiveCommand();
  dl.respondCommand();

  // delay(5000);
  // char dataAndTime[20];
  // dl.serial.println(dl.lastSave.tostr(dataAndTime));
  // dl.serial.println((dl.lastSave + TimeDelta(10)).tostr(dataAndTime));
  // dl.serial.println(dl.rtc.getDateTime());
  // dl.lastSave = dl.rtc.now();

//  if((dl.rtc.now() - dl.lastSave) >= TimeDelta(5)) {
//    testPrint();
//    dl.lastSave = dl.rtc.now();
//  }
}
