#ifndef SDCARD_H
#define SDCARD_H

#include "HardwarePins.h"

#include <SdFat.h>
#include <sdios.h>

#define SD_FAT_TYPE 0
const uint8_t SD_CS_PIN = 44;
#define SPI_CLOCK SD_SCK_MHZ(4)
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SPI_CLOCK)

#if SD_FAT_TYPE == 0
SdFat sd;
File file;
#elif SD_FAT_TYPE == 1
SdFat32 sd;
File32 file;
#elif SD_FAT_TYPE == 2
SdExFat sd;
ExFile file;
#elif SD_FAT_TYPE == 3
SdFs sd;
FsFile file;
#else  // SD_FAT_TYPE
#error Invalid SD_FAT_TYPE
#endif  // SD_FAT_TYPE

// Serial print stream
// ArduinoOutStream cout(Serial);

class SDCardRW
{
private:
  HardwarePins &hardPins;
  HardwareSerial &serial;
  File file;
  SdFat &card = sd;

public:
  SDCardRW(HardwarePins &hardPins, HardwareSerial &serial);
  ~SDCardRW();

  int setupSDCard();
  void readWriteTest(const char* fileName);
  void WriteHumidity(char* dataloggerID, const char* dateTime, int sensorID, float soilMoisture, float ambientHumidity, float roomTemperature,  const char* fileName);
  bool isCardPresent();
  bool initializeCard();
  void createFile(const char* fileName);

  bool cardInitialized = false;

};

SDCardRW::SDCardRW(HardwarePins &hardPins, HardwareSerial &serial)
   : hardPins{hardPins}, serial{serial}
{
}

SDCardRW::~SDCardRW(){}

bool SDCardRW::isCardPresent()
{
  if(digitalRead(hardPins.noSDCardPin) == HIGH) // No card
  {
    digitalWrite(hardPins.noCardLed, HIGH);
    cardInitialized = false;
    return 0;
  }
  else // With card
  {
    digitalWrite(hardPins.noCardLed, LOW);
    return 1;
  }
}

bool SDCardRW::initializeCard()
{
  // static uint8_t count = 0;
  digitalWrite(hardPins.CS_SDCARD, HIGH);
  digitalWrite(hardPins.CS_MCP3008, HIGH);

  // if(count == 32){
  //   digitalWrite(hardPins.noCardLed, !digitalRead(hardPins.noCardLed));
  //   count = 0;
  // }
  // ++count;

  // digitalWrite(hardPins.cardReset, LOW);
  // delay(1000);
  // digitalWrite(hardPins.cardReset, HIGH);
  // digitalWrite(52, LOW);
  // digitalWrite(51, LOW);
  // digitalWrite(50, LOW);
  // delay(1000);
  cardInitialized = card.begin(hardPins.CS_SDCARD, 4);
  // delay(1000);

  digitalWrite(hardPins.CS_SDCARD, HIGH);
  digitalWrite(hardPins.CS_MCP3008, HIGH);

  return cardInitialized;
}

int SDCardRW::setupSDCard()
{
  if(isCardPresent() && !cardInitialized)
  {
    serial.println("  Inicializando SDCard ");
    cardInitialized = initializeCard();
  }

  if(cardInitialized)
  {
    serial.println("    * SDCard inicializado com sucesso");
    digitalWrite(hardPins.hardwareOkLed, HIGH);
    digitalWrite(hardPins.noCardLed, LOW);
    return cardInitialized; // Card initialized
  }
  else{
    digitalWrite(hardPins.hardwareOkLed, LOW);
    digitalWrite(hardPins.noCardLed, HIGH);
    serial.println("    ! Falha no SDCard");
    serial.print("    Tentando inicializar SDCard .");

    while(!cardInitialized){
      cardInitialized = initializeCard();
      delay(250);
      serial.print(".");
    }
    return cardInitialized; // Card NOT initialized
  }
}

void SDCardRW::createFile(const char* fileName)
{
  if (!card.exists(fileName))
  {
    serial.print("Creating file: ");
    serial.println(fileName);

    file = card.open(fileName, FILE_WRITE);
    if(file){
      file.print("DataloggerID");
      file.print(";");
      file.print("Data");
      file.print(";");
      file.print("Horario");
      file.print(";");
      file.print("SensorID");
      file.print(";");
      file.print("UmidadeSolo");
      file.print(";");
      file.print("UmidadeAmbiente");
      file.print(";");
      file.println("TemperaturaAmbiente");
    }else{
      serial.println("Não foi possível criar o arquivo");
      digitalWrite(hardPins.failCardLed, HIGH);
      initializeCard();
    }
    file.close();
    delay(150);
  }
}

void SDCardRW::WriteHumidity(char* dataloggerID, const char* dateTime, int sensorID, float soilMoisture, float ambientHumidity, float roomTemperature,  const char* fileName)
{
  const char tmp[12];

  if (card.exists(fileName))
  {

    file = card.open(fileName, FILE_WRITE);
    if (file) {
      digitalWrite(hardPins.failCardLed, LOW);
      digitalWrite(hardPins.hardwareOkLed, LOW);
      // serial.print("Writing on file: ");
      // serial.println(fileName);

      serial.print("save;");
      serial.print(dataloggerID);
      serial.print(";");
      strncpy(tmp, "\0\0\0\0\0\0\0\0\0\0\0\0", 12);
      strncpy(tmp, dateTime, 10); //copy date YYYY/MM/DD from dateTime "YYYY/MM/DD hh:mm:ss" to tmp
      serial.print(tmp);
      serial.print(";");
      strncpy(tmp, "\0\0\0\0\0\0\0\0\0\0\0\0", 12);
      strncpy(tmp, dateTime+11, 9); //copy time hh:mm:ss from dateTime "YYYY/MM/DD hh:mm:ss" to tmp
      serial.print(tmp);
      serial.print(";");
      serial.print(sensorID);
      serial.print(";");
      serial.print(soilMoisture);
      serial.print(";");
      serial.print(ambientHumidity);
      serial.print(";");
      serial.println(roomTemperature);

      file.print(dataloggerID);
      file.print(";");
      strncpy(tmp, dateTime, 10); //copy date YYYY/MM/DD from dateTime "YYYY/MM/DD hh:mm:ss" to tmp
      file.print(tmp);
      file.print(";");
      strncpy(tmp, dateTime+11, 9); //copy time hh:mm:ss from dateTime "YYYY/MM/DD hh:mm:ss" to tmp
      file.print(tmp);
      file.print(";");
      file.print(sensorID);
      file.print(";");
      file.print(soilMoisture);
      file.print(";");
      file.print(ambientHumidity);
      file.print(";");
      file.println(roomTemperature);

      file.close();
      digitalWrite(hardPins.hardwareOkLed, HIGH);
    } else {
      // if the file didn't open, print an error:
      serial.print("error opening ");
      serial.println(fileName);
      digitalWrite(hardPins.failCardLed, HIGH);
      // delay(250);
      initializeCard();
      // delay(250);
    }
  }else{
    createFile(fileName);
  }
}

void SDCardRW::readWriteTest(const char* fileName)
{
  // re-open the file for reading:
  if (card.exists(fileName))
  {
    file = card.open(fileName, FILE_WRITE);
    if (file) {
      serial.println("file ok");

      // read from the file until there's nothing else in it:
      serial.write(file.read());
      
      // close the file:
      file.close();
    } else {
      // if the file didn't open, print an error:
      serial.println("error opening file");
    }
  }
}

#endif // SDCARD_H

