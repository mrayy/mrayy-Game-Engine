
#ifndef Kondo_h
#define Kondo_h


#include "Arduino.h"
#include "HardwareSerial.h"


static constexpr int ICS_FALSE = -1;

class KondoClass {
public:
  void  begin(long baud, HardwareSerial* serial);
  void  end();

  int setFree(byte servoID);    //サーボ脱力＋現在値読込

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


  //サーボIDリミット
  static byte idMax(byte id);

  ////サーボ可動範囲　パラメータ範囲　リミット設定
  static bool maxMin(int maxPos, int minPos, int val);

   //角度変換 POSから角度へ変換
   static  int degPos(float deg);
   //角度変換 角度からPOSへ変換
   static float posDeg(int pos);

   //角度変換 x100 POSから角度へ変換
   static int degPos100(int deg);
   //角度変換 x100 角度からPOSへ変換
   static int posDeg100(int pos);

// private area
private:
  void enHigh(){}
  void enLow(){}
  bool synchronize(byte *txBuf, byte txLen, byte *rxBuf, byte rxLen);

  HardwareSerial *_serial;
};

#endif    // Kondo_h
