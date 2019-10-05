
#include "Kondo.h"


static constexpr int MIN_ID = 0;
static constexpr int MAX_ID = 31;

static constexpr int MIN_POS = 3500;
static constexpr int MAX_POS = 11500;


static constexpr float ANGLE_F_FALSE = 9999.9;
static constexpr int   ANGLE_I_FALSE = 0x7FFF;

static constexpr int MAX_127 = 127;
static constexpr int MAX_63 = 63;
static constexpr int MIN_1 = 1;

//  static const float MIN_DEG = -135.0;
static constexpr float MIN_DEG = -180.0;
//  static const float MAX_DEG = 135.0;
static constexpr float MAX_DEG = 180.0;

//static const int MIN_100DEG = -13500;
static constexpr int MIN_100DEG = -18000;
//static const int MAX_100DEG = 13500;
static constexpr int MAX_100DEG = 18000;




// Kondo begin with Arduino Uno
void KondoClass::begin(long baud,int enPin, HardwareSerial* serial)
{
  IcsHardSerialClass::begin(serial,enPin,baud,10);
  /*_serial->setRX(rx);
  _serial->setTX(tx);
  _serial->begin(baud);
  _serial=serial;
  _serial->begin(baud,SERIAL_8E1);
  _serial->setTimeout(50);*/
}
// Kondo end
void KondoClass::end()
{
  icsHardSerial->end();
}

 unsigned int KondoClass::setPosition(byte id, int pos) {

     return IcsBaseClass::setPos(id,pos);
}
// get Position
 unsigned int KondoClass::getPosition(byte id) {

     return IcsBaseClass::getPos(id);


}

float KondoClass::getAngle(byte servoID) {
  return posDeg(getPosition(servoID));
}

float KondoClass::setAngle(byte servoID,float angle) {
  int v=setPosition(servoID,degPos(angle));
  if(v==ICS_FALSE)
    return -1000;
  return posDeg(v);
}


int KondoClass::setSpeed(byte id, unsigned int spd)
{
  return IcsBaseClass::setSpd(id,spd);
}
int KondoClass::setCurrentLimit(byte id, unsigned int curlim)
{
  return IcsBaseClass::setCur(id,curlim);
}
int KondoClass::setTemperature(byte id, unsigned int tmplim)
{
  return IcsBaseClass::setTmp(id,tmplim);

}
int KondoClass::getSpeed(byte id)
{

    return IcsBaseClass::getSpd(id);;
}
int KondoClass::getCurrent(byte id)
{

  return IcsBaseClass::getCur(id);

}
int KondoClass::getTemperature(byte id)
{
  return IcsBaseClass::getTmp(id);

}



// Private Methods //////////////////////////////////////////////////////////////

/*
bool KondoClass::synchronize(byte *txBuf, byte txLen, byte *rxBuf, byte rxLen)
{
  int rxSize; //受信数

  _serial->flush(); //待つ
  enHigh(); //送信切替
  _serial->write(txBuf, txLen);
  _serial->flush();   //待つ
  //delayMicroseconds(600);
  enLow();  //受信切替


    if (_serial->available() > 0) //受信バッファを消す
    {
      for(int i=0;i<rxLen;++i)
      {
          rxBuf[i]=_serial->read();   //空読み
      }
      rxSize=rxLen;
    }
  //rxSize = _serial->readBytes(rxBuf, rxLen);

  if (rxSize != rxLen) //受信数確認
  {
    return false;
  }
  return true;


}

*/
