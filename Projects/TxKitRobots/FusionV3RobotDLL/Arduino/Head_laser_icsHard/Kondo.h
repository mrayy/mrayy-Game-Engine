
#ifndef Kondo_h
#define Kondo_h


#include "Arduino.h"
#include "IcsHardSerialClass.h"


static constexpr int ICS_FALSE = -1;

class KondoClass:public IcsHardSerialClass {
public:
  void  begin(long baud,int enPin, HardwareSerial* serial);
  void  end();
  unsigned int setPosition(byte servoID, int pos);    //目標値設定
  unsigned int getPosition(byte id);   //現在位置読込　    ※ICS3.6以降で有効

  int setSpeed(byte id, unsigned int spd);      //スピード書込   1～127  1(遅い) <=>    127(速い)
  int setCurrentLimit(byte id, unsigned int curlim);   //電流制限値書込 1～63   1(低い) <=>    63 (高い)
  int setTemperature(byte id, unsigned int tmplim);   //温度上限書込   1～127  127(低温） <=> 1(高温)
  //各種パラメータ読込み
  int getSpeed(byte id);   //スピード読込      1～127  1(遅い)<=>     127(速い)
  int getCurrent(byte id);   //電流値読込        63←0 | 64→127
  int getTemperature(byte id);   //現在温度読込      127(低温）<=>　0(高温)

  float setAngle(byte servoID,float angle);
  float getAngle(byte servoID);


// private area
private:
 // bool synchronize(byte *txBuf, byte txLen, byte *rxBuf, byte rxLen);

 // HardwareSerial *_serial;
};

#endif    // Kondo_h
