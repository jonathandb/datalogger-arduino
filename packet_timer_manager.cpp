#include "packet_timer_manager.h"
#include "stdlib.h"
#include "string.h"
#include <SD.h>

PacketTimerManager::PacketTimerManager(Configuration *configuration) {
  _configuration = configuration;
  for(int i = 0; i < MAX_NUMBER_OF_TYPES; i++) {
    sensorTypesToCheck[i] = false;
  }
  
  createTimersFromConfiguration();
}


void PacketTimerManager::createTimersFromConfiguration() {
  for(int i = 0; i < MAX_NUMBER_OF_TYPES; i++) {
    if(_configuration->parameters.types[i] == '0') {  //if type = 0 don't check time for it
      nextTimesToCheck[i] = 0;
    } else {
      /*
      set nextTimeToCheck[i] to the first following time interval according to the current time   
      */
      
      time_t currentNow = now();
      time_t nowDate = currentNow - (currentNow % (3600*24));
      
      int nextTimeToCheck = nowDate + _configuration->parameters.timeToStartToCheckEachType[i] % (3600*24);
      int timeInterval = _configuration->parameters.timeIntervalToCheckEachType[i] % (3600*24);
      if(timeInterval <= 0) {
        timeInterval = 3600*24;
      }
      
      
      while(nextTimeToCheck >= currentNow + timeInterval) {        
        nextTimeToCheck -= timeInterval;
      }
            
      while(nextTimeToCheck <= currentNow) {
        nextTimeToCheck += timeInterval;
      }      
      nextTimesToCheck[i] = nextTimeToCheck;
    }
  }  
}

bool PacketTimerManager::checkTimers() {
  bool timerActive = false;
  time_t currentNow = now();
  for(unsigned short i = 0; i < MAX_NUMBER_OF_TYPES; i++) {
    if(nextTimesToCheck[i] < currentNow) {
      //read sensor
      sensorTypesToCheck[i] = true;
      
      //set next time to check
      unsigned int timeInterval = _configuration->parameters.timeIntervalToCheckEachType[i] % (3600*24);
      if(timeInterval <= 0) {
        timeInterval = 3600*24;
      }
      
      while(nextTimesToCheck[i] >= currentNow + timeInterval) {        
        nextTimesToCheck[i] -= timeInterval;
      }
            
      while(nextTimesToCheck[i] <= currentNow) {
        nextTimesToCheck[i] += timeInterval;
      }
      timerActive = true;
    }
  }
  return timerActive;
}
