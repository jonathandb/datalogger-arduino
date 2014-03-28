
//#include "SIM900.h"
//#include "inetGSM.h"
#include <Time.h>
#include <arduino.h>
#include <SD.h>
#include <EEPROM.h>
#include "packet_manager.h"
#include "packet_timer_manager.h"
#include <MemoryFree.h>
#include <ModbusMaster.h>

PacketManager packetManager;

Configuration configuration;

PacketTimerManager packetTimerManager(&configuration);


Packet p;
bool stream = false;
unsigned short sensorData[25];

void setup() {
    //check configuration
    
    Serial.begin(9600);
    
    delay(100);
    
    
    for(int i = 0; i < 22; i++) {
      sensorData[i] = i*100;
    }
    
    unsigned short *pSensorData = sensorData;
    
    unsigned short numberOfSensorData = 22;
    packetManager.createPacket(&p, 'a', now(), pSensorData, numberOfSensorData);
    
    //InetGSM inet;
    //File dataFile = SD.open("datalog.txt", FILE_WRITE);
}

void loop() {  
    if(!stream) {
      char json[MAX_JSON_CHAR_STREAM_BUFFER];
      
      bool gfa = packetManager.packetJsonCreatorStream(json, &p);
      
      Serial.println(json);
      if(!gfa) {
        stream = true;
        Serial.println("complete!");
        
      }
    }
  
    if(packetTimerManager.checkTimers()) {
      //sensors to check!
      collectSensorsToCheck();
    }
    
    delay(1000);
    Serial.print("freeMemory()=");
    Serial.println(freeMemory());
    
    
    
    //Serial.println("Sensors to check: ");
    //for(int i = 0; i < MAX_NUMBER_OF_TYPES; i++) {
      //Serial.println(packetTimerManager.sensorTypesToCheck[i]);
    //}
    
    
    //Serial.print("test");

}

void collectSensorsToCheck() {
  for(unsigned short i = 0;  i < MAX_NUMBER_OF_TYPES; i++) {
    if(packetTimerManager.sensorTypesToCheck[i]) {
      //sensor type to check is parameters.types[i]
      //first check h mow
      unsigned short numberOfSensorsToCheck = 0;
      
      for(unsigned short j = 0; j < MAX_NUMBER_OF_MODBUS_SLAVES; j++) {
        if(configuration.parameters.modbusSlaveTypes[j] == configuration.parameters.types[i]) {
          //modbus slave to check: configuration.parameters.modbusSlaveAddresses[j]
          Serial.print("check modbus:");
          Serial.println(configuration.parameters.modbusSlaveAddresses[j]);
          numberOfSensorsToCheck += 1;         
        }
      }
      
      for(unsigned short j = 0; j < MAX_NUMBER_OF_I2C_SLAVES; j++) {
        if(configuration.parameters.i2cSlaveTypes[j] == configuration.parameters.types[i]) {
          //i2c slave to check: configuration.parameters.i2cSlaveAddresses[j]
          Serial.print("check i2c:");
          Serial.println(configuration.parameters.i2cSlaveAddresses[j]);  
          numberOfSensorsToCheck += 1;
        }
      }
      
      unsigned short sensorData[numberOfSensorsToCheck];
      unsigned short currentSensor = 0;
      
      for(unsigned short j = 0; j < MAX_NUMBER_OF_MODBUS_SLAVES; j++) {
        if(configuration.parameters.modbusSlaveTypes[j] == configuration.parameters.types[i]) {
          //modbus slave to check: configuration.parameters.modbusSlaveAddresses[j]
          //sensorData[currentSensor] = getModbusSensorData(configuration.parameters.modbusSlaveAddresses[j]);
          currentSensor++;
        }
      }
      
      for(unsigned short j = 0; j < MAX_NUMBER_OF_I2C_SLAVES; j++) {
        if(configuration.parameters.i2cSlaveTypes[j] == configuration.parameters.types[i]) {
          //i2c slave to check: configuration.parameters.i2cSlaveAddresses[j]
          //sensorData[currentSensor] = getI2cSensorData(configuration.parameters.i2cSlaveAddresses[j]);
          currentSensor++;
        }
      }
      
    }
      
    }
  }

  unsigned short getModbusSensorData(unsigned short modbusAddress, unsigned short) {
    
  }
  
  unsigned short getI2cSensorData(unsigned short i2cSlaveAddress) {
    
  }
  


