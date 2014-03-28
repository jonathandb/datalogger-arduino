/*

When a PacketTimerManager instance is initialised:
for every type a time is set in nextTimesToCheck which is the next time the sensors of the type needs to be checked

When checkTimers() is called:
every time in nextTimesToCheck is being compared to the current time. When one of the times is bigger then the current time, 
the time interval of the type is added to it and the corresponding sensorTypesToCheck is being set to true

*/

#include <Time.h>
#include "configuration.h"

typedef struct Timer {
  time_t startTimer;
  time_t timeInterval;
};

class PacketTimerManager {
  private:

    Configuration *_configuration;
  public:
    time_t nextTimesToCheck[MAX_NUMBER_OF_TYPES];
    bool sensorTypesToCheck[MAX_NUMBER_OF_TYPES];
    PacketTimerManager(Configuration *configuration);
    void createTimersFromConfiguration();
    bool checkTimers();
};
