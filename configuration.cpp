/* Based on http://playground.arduino.cc/Code/EEPROMLoadAndSaveSettings footswitch 2012-03-05

 */
#include "configuration.h"
#include <EEPROM.h>
#include "string.h"

Configuration::Configuration() {
  loadConfig();
}

void Configuration::loadConfig() {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if (//EEPROM.read(CONFIG_START + sizeof(programVersion) - 1) == parameters.programVersion[3] // this is '\0'
      EEPROM.read(CONFIG_START + sizeof(parameters) - 2) == CONFIG_VERSION[2] &&
      EEPROM.read(CONFIG_START + sizeof(parameters) - 3) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + sizeof(parameters) - 4) == CONFIG_VERSION[0])
  { // reads settings from EEPROM
    for (unsigned int t=0; t<sizeof(parameters); t++) {
      *((char*)&parameters + t) = EEPROM.read(CONFIG_START + t);
    }
    loadSuccess = true;
  } else {
    // settings aren't valid! will overwrite with default settings
    saveDefaultConfig();
    loadSuccess = false;
  }
}

void Configuration::saveDefaultConfig() {  
  for(int i = 0; i < MAX_NUMBER_OF_MODBUS_SLAVES; i++) {
    parameters.modbusSlaveAddresses[i] = i + 1;
    parameters.modbusSlaveTypes[i] = 2; 
  }
  
  for(int i = 0; i < MAX_NUMBER_OF_I2C_SLAVES; i++) {
    parameters.i2cSlaveAddresses[i] = i + 1;
    parameters.i2cSlaveTypes[i] = 4; 
  }
  
  for(int i = 0; i < MAX_NUMBER_OF_TYPES; i++) {
    parameters.types[i] = i + 1;
    parameters.timeToStartToCheckEachType[i] = i*10;
    parameters.timeIntervalToCheckEachType[i]= i*10;
  }
  
  strcpy(parameters.serverAddress, "www.google.be\0");
  strcpy(parameters.programVersion, CONFIG_VERSION);
  saveConfig();
}

void Configuration::saveConfig() {
  for (unsigned int t=0; t<sizeof(parameters); t++)
  { // writes to EEPROM"
    EEPROM.write(CONFIG_START + t, *((char*)&parameters + t));
    // and verifies the data
    if (EEPROM.read(CONFIG_START + t) != *((char*)&parameters + t))
    {
      // error writing to EEPROM
    }
  }
}

void Configuration::parseJsonConfig() {
  
}


