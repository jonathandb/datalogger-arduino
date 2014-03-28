/*

A packet file has following naming convention: DDMMYYXX.Z
DDMMYY: date
XX: type
Z: number of same type on same date (in case of reboot and multiple files)


A packet file consists of:
XX\n
timeDate(4 bytes)sensorData(2 * XX bytes) \n

*/

#include <SD.h>
#include "Time.h"
#include "Arduino.h"


#define MAX_JSON_CHAR_STREAM_BUFFER 52


typedef unsigned char* RawPacket;


typedef struct Packet {
    char type;   //ie houses, meteo. (different time interval) (1byte)
    time_t timeDate;  // (4 byte)
    unsigned short numberOfSensorData;
    unsigned short *sensorData;  //(2 byte * x)
    unsigned short *sensorId; //parallel to sensorData
};

class PacketManager {
  private:
    RawPacket rawPacketPiece;
    char tempByte;
    unsigned short numberOfSensorData;
    unsigned short packetSize;
    unsigned int loopCounter;
    unsigned int streamingPacketPosition;
    File streamingFile;
    File writingFile;
    unsigned int sdPosition;
    unsigned short jsonStreamSensorPosition;
    bool parsePacket(char b);
    Packet *streamPacket;
  public:
    PacketManager();
    void initPacketFile(char type, time_t timeDate, unsigned short numberOfSensorData);
    void writePacket(char type, time_t timeDate, unsigned short sensorData[], unsigned short numberOfSensorData);
    void createPacket(Packet *packet, char type, time_t timeDate, unsigned short *sensorData, unsigned short numberOfSensorData);
    bool openFileFromDay(unsigned short sday, unsigned short smonth, unsigned short syear, unsigned char type);
    bool loadNextPacketFromDay(Packet *packet);
    bool streamingPacketBusy;
    bool streamingJsonBusy;
    int saveToEEPROM(Packet *packet); //warn when EEPROM almost full
    void loadFromEEPROM(Packet packet[]);
    void clearEEPROM();
    bool packetJsonCreatorStream(char *json, Packet *packet);
};
