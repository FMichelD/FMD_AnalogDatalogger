#include "HardwarePins.h"
#include "SDCardRW.h"
#include "RtcTimer.h"
#include "DHT22.h"
#include <Adafruit_MCP3008.h>

#include <Arduino.h>
#include <HardwareSerial.h>
#include <Regexp.h>
#include <EEPROM.h>


/**
 * @brief 
 * 
 */

class Datalogger
{
private:

public:
  Datalogger();
  ~Datalogger();

  void setupHardware();
  void readNativeAnalogInputs();

  void receiveCommand();
  void respondCommand();

  void writeIntIntoEEPROM(int address, int number);
  int readIntFromEEPROM(int address);

  HardwarePins hardPins;
  HardwareSerial &serial = Serial;  
  SDCardRW card;
  RtcTimer rtc;
  Adafruit_MCP3008 adc;
  DHTSensor dht22;
  DateTime lastSave;
  MatchState ms;
  uint16_t registerInterval = 10;
  int addrRegisterInterval = 0;

  static const byte numChars = 32;
  char receivedChars[numChars];   // an array to store the received data
  boolean newData = false;
  char ID[4];
  char fileName[256] = "CheckSourceCode.csv";
};

Datalogger::Datalogger()
  :card{hardPins, serial}, rtc{hardPins, serial}, dht22{hardPins, serial}
{
}

Datalogger::~Datalogger()
{
}

void Datalogger::setupHardware() 
{
  serial.begin(115200);
  serial.println("Inicializando...");

  pinMode(hardPins.CS_MCP3008, OUTPUT);
  digitalWrite(hardPins.CS_MCP3008, HIGH);

  // configure SDCard pins
  pinMode(hardPins.CS_SDCARD, OUTPUT);
  digitalWrite(hardPins.CS_SDCARD, HIGH);

  pinMode(hardPins.noSDCardPin, INPUT_PULLUP);

  pinMode(hardPins.noCardLed, OUTPUT);
  digitalWrite(hardPins.noCardLed, LOW);

  pinMode(hardPins.failCardLed, OUTPUT);
  digitalWrite(hardPins.failCardLed, LOW);

  pinMode(hardPins.ss, OUTPUT);
  digitalWrite(hardPins.ss, HIGH);

  pinMode(hardPins.cardReset, OUTPUT);
  digitalWrite(hardPins.cardReset, HIGH);

  // configure general pins
  pinMode(hardPins.hardwareOkLed, OUTPUT);
  digitalWrite(hardPins.hardwareOkLed, LOW);

  // configura analogInputs
  for(uint8_t i = 0; i <= hardPins.numAnalogInputs; ++i)
  {
    pinMode(hardPins.arduinoAnalogInputs, INPUT);
  }

  // card.setupSDCard();

  rtc.setup();
  
  dht22.setup();

  adc.begin(hardPins.CS_MCP3008);

  writeIntIntoEEPROM(addrRegisterInterval, registerInterval);

  serial.println("\n*** Inicialização concluída com sucesso ***");
  digitalWrite(hardPins.hardwareOkLed, LOW);
}

void Datalogger::readNativeAnalogInputs()
{
  float soilMoisture;
  // Read native analog ports
  for(uint8_t i = 0; i < hardPins.numAnalogInputs; ++i)
  {
    soilMoisture = analogRead(hardPins.arduinoAnalogInputs[i]);

    card.WriteHumidity(ID, rtc.getDateTime(), i+1, soilMoisture,   dht22.readHumidity(), dht22.readTemperature(), fileName);
  } 

  // Read MC3008 analog ports
  for(uint8_t channel = 0; channel < 2; ++channel)
  {
    soilMoisture = adc.readADC(channel);

    card.WriteHumidity(ID, rtc.getDateTime(), channel+17, soilMoisture,   dht22.readHumidity(), dht22.readTemperature(), fileName);
  } 

}

void Datalogger::receiveCommand() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (Serial.available() > 0 && newData == false) {
        rc = serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void Datalogger::respondCommand() {
    if (newData == true) {
      serial.print("receivedChars: ");
      serial.println(receivedChars);
      
      if (strcmp(receivedChars, "AT+ID") == 0) {
        serial.print("Arduino recebeu o commando: ");
        serial.println(receivedChars);

        serial.print("#");
        serial.println(ID);
      }

      ms.Target(receivedChars);
      if ( ms.Match("AT%+TIME%#") > 0) {
        // Serial.println (ms.MatchLength);
        // serial.println (ms.MatchStart);
        size_t len = strlen(receivedChars);
        // serial.print("receivedChars len: ");
        // serial.println(len);

        char logTime[12];
        strncpy(logTime, receivedChars+ms.MatchLength, len - ms.MatchLength);
        serial.print("Configurando um tempo de: ");
        serial.print(atoi(logTime) / 60);
        serial.println(" minutos");
        registerInterval = atoi(logTime);
        writeIntIntoEEPROM(addrRegisterInterval, registerInterval);
      }

      newData = false;
      memset(receivedChars, '\0', sizeof(receivedChars));  
    }
}

void Datalogger::writeIntIntoEEPROM(int address, int number)
{ 
  byte byte1 = number >> 8;
  byte byte2 = number & 0xFF;
  EEPROM.write(address, byte1);
  EEPROM.write(address + 1, byte2);
}

int Datalogger::readIntFromEEPROM(int address)
{
  byte byte1 = EEPROM.read(address);
  byte byte2 = EEPROM.read(address + 1);
  return (byte1 << 8) + byte2;
}