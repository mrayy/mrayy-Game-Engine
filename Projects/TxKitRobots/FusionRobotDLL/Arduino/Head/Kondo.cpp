
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
void KondoClass::begin(long baud, HardwareSerial* serial)
{
	/*_serial->setRX(rx);
	_serial->setTX(tx);
	_serial->begin(baud);*/
  _serial=serial;
  _serial->begin(baud,SERIAL_8E1);
  _serial->setTimeout(50);
}
// Kondo end
void KondoClass::end()
{
	_serial->end();
}

int KondoClass::setFree(byte id)
{

    byte txCmd[3];
    byte rxCmd[3];
    unsigned int rePos;
    bool flg;

    if (id != idMax(id)) //範囲外の時
    {
      return ICS_FALSE;
    }

    txCmd[0] = 0x80 | id;    // CMD
    txCmd[1] = 0;
    txCmd[2] = 0;

    //送受信
    flg = synchronize(txCmd, sizeof txCmd, rxCmd, sizeof rxCmd);
    if (flg == false)
    {
      return ICS_FALSE;
    }

    rePos = ((rxCmd[1] << 7) & 0x3F80) + (rxCmd[2] & 0x007F);

    return rePos;
}

 unsigned int KondoClass::setPosition(byte id, int pos) {

     byte txCmd[3];
     byte rxCmd[6];
     unsigned int rePos;
     bool flg;


     if ((id != idMax(id)) || ( ! maxMin(MAX_POS, MIN_POS, pos)) ) //範囲外の時
     {
       return ICS_FALSE;
     }

     txCmd[0] = 0x80 + id;               // CMD
     txCmd[1] = (byte)((pos >> 7) & 0x7F);   // POS_H
     txCmd[2] = (byte)(pos & 0x7F);          // POS_L

     //送受信



     flg = synchronize(txCmd, sizeof txCmd, rxCmd, sizeof rxCmd);
     if (flg == false)
     {
       return ICS_FALSE;
     }



     rePos = (rxCmd[4] << 7)  | rxCmd[5];

     return rePos;
}
// get Position
 unsigned int KondoClass::getPosition(byte id) {

     byte txCmd[2];
     byte rxCmd[4];
     unsigned int reData;
     bool flg;
     if (id != idMax(id)) //範囲外の時
     {
       return ICS_FALSE;
     }
     txCmd[0] = 0xA0 | id;    // CMD
     txCmd[1] = 0x05;         // 角度読出し


     //送受信
     flg = synchronize(txCmd, sizeof txCmd, rxCmd, sizeof rxCmd);
     if (flg == false)
     {
       return ICS_FALSE;
     }

     reData = ((rxCmd[2] << 7) & 0x3F80) + (rxCmd[3] & 0x007F);

     return reData;


}

float KondoClass::getAngle(byte servoID) {
	return posDeg(getPosition(servoID));
}

float KondoClass::setAngle(byte servoID,float angle) {
	return posDeg(setPosition(servoID,degPos(angle)));
}


int KondoClass::setSpeed(byte id, unsigned int spd)
{
  byte txCmd[3];
  byte rxCmd[3];
  unsigned int reData;
  bool flg;

  if ((id != idMax(id)) || ( ! maxMin(MAX_127, MIN_1, spd)) ) //範囲外の時
  {
    return ICS_FALSE;
  }


  txCmd[0] = 0xC0 + id;      // CMD
  txCmd[1] = 0x02;           // SC スピード
  txCmd[2] = spd;            // スピード

  //送受信
  flg = synchronize(txCmd, sizeof txCmd, rxCmd, sizeof rxCmd);
  if (flg == false)
  {
    return ICS_FALSE;
  }

  reData = rxCmd[2];

  return reData;
}
int KondoClass::setCurrentLimit(byte id, unsigned int curlim)
{
  byte txCmd[3];
  byte rxCmd[3];
  unsigned int reData;
  bool flg;

  if ((id != idMax(id)) || ( ! maxMin(MAX_63, MIN_1, curlim)) ) //範囲外の時
  {
    return ICS_FALSE;
  }

  txCmd[0] = 0xC0 + id;                     // CMD
  txCmd[1] = 0x03;                          // SC 電流値
  txCmd[2] = curlim;                        // 電流リミット値


  //送受信
  flg = synchronize(txCmd, sizeof txCmd, rxCmd, sizeof rxCmd);
  if (flg == false)
  {
    return ICS_FALSE;
  }

  reData = rxCmd[2];

  return reData;
}
int KondoClass::setTemperature(byte id, unsigned int tmplim)
{
  byte txCmd[3];
  byte rxCmd[3];
  unsigned int reData;
  bool flg;
  if ((id != idMax(id)) || ( ! maxMin(MAX_127, MIN_1, tmplim)) ) //範囲外の時
  {
    return ICS_FALSE;
  }


  txCmd[0] = 0xC0 | id;                      // CMD
  txCmd[1] = 0x04;                           // SC 温度値
  txCmd[2] = tmplim;                         // 温度リミット値

  //送受信
  flg = synchronize(txCmd, sizeof txCmd, rxCmd, sizeof rxCmd);
  if (flg == false)
  {
    return ICS_FALSE;
  }

  reData = rxCmd[2];

  return reData;

}
int KondoClass::getSpeed(byte id)
{
    byte txCmd[2];
    byte rxCmd[3];
    unsigned int reData;
    bool flg;
    if (id != idMax(id)) //範囲外の時
    {
      return ICS_FALSE;
    }
    txCmd[0] = 0xA0 + id;    // CMD
    txCmd[1] = 0x02;         // SC スピード


    //送受信
    flg = synchronize(txCmd, sizeof txCmd, rxCmd, sizeof rxCmd);
    if (flg == false)
    {
      return ICS_FALSE;
    }

    reData = rxCmd[2];

    return reData;
}
int KondoClass::getCurrent(byte id)
{
  byte txCmd[2];
  byte rxCmd[3];
  unsigned int reData;
  bool flg;
  if (id != idMax(id)) //範囲外の時
  {
    return ICS_FALSE;
  }
  txCmd[0] = 0xA0 + id;    // CMD
  txCmd[1] = 0x03;         // SC 電流値


  //送受信
  flg = synchronize(txCmd, sizeof txCmd, rxCmd, sizeof rxCmd);
  if (flg == false)
  {
    return ICS_FALSE;
  }

  reData = rxCmd[2];

  return reData;

}
int KondoClass::getTemperature(byte id)
{
  byte txCmd[2];
  byte rxCmd[3];
  unsigned int reData;
  bool flg;
  if (id != idMax(id)) //範囲外の時
  {
    return ICS_FALSE;
  }
  txCmd[0] = 0xA0 + id;    // CMD
  txCmd[1] = 0x04;         // SC 温度値


  //送受信
  flg = synchronize(txCmd, sizeof txCmd, rxCmd, sizeof rxCmd);
  if (flg == false)
  {
    return ICS_FALSE;
  }

  reData = rxCmd[2];

  return reData;

}



// Private Methods //////////////////////////////////////////////////////////////


bool KondoClass::synchronize(byte *txBuf, byte txLen, byte *rxBuf, byte rxLen)
{
	int rxSize; //受信数

	_serial->flush(); //待つ
	enHigh(); //送信切替
	_serial->write(txBuf, txLen);
	_serial->flush();   //待つ
  delayMicroseconds(600);
/*
	while (_serial->available() > 0) //受信バッファを消す
	{
		// buff = icsSerial->read();	//空読み
		_serial->read();		//空読み
	}*/

	enLow();  //受信切替


  	if (_serial->available() > 0) //受信バッファを消す
  	{
      for(int i=0;i<rxLen;++i)
      {
  		    rxBuf[i]=_serial->read();		//空読み
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


//サーボID範囲 /////////////////////////////////////////////////////////////////////////////////////////////
/**
* @brief サーボIDが範囲内か見る
* @param[in] id
* @return 送られてきたid
* @retval 0xFF 範囲外
**/
byte KondoClass::idMax(byte id)
{
  if ((char)id < MIN_ID) {
    id = 0xFF;
  }
  if (id > MAX_ID) {
    id = 0xFF;
  }
  return id;
}



//サーボ可動範囲　パラメータ範囲　リミット設定 //////////////////////////////////////////////////////////////
/**
* @brief ポジションデータの最大、最小値の範囲に収める
* @param[in] maxPos 最大値
* @param[in] minPos 最小値
* @param[in,out] val 現在値
* @return リミットがかかったポジションデータ
**/
bool KondoClass::maxMin(int maxPos, int minPos, int val)
{
  if (val > maxPos) {
    return false;
  }
  if (val < minPos) {
    return false;
  }
  return true;
}



//角度変換　角度からPOSへ////////////////////////////////////////////////////////////////////////////////////
/**
* @brief 角度データ(float型)をポジションデータに変換
* @param[in] deg 角度(deg)(float型)
* @return ポジションデータ
* @retval -1 範囲外
**/
int KondoClass::degPos(float deg)
{
  if (deg > MAX_DEG) {
    return -1;
  }
  if (deg < MIN_DEG) {
    return -1;
  }
  int pos = deg * 29.633;
  pos = pos + 7500;
  return pos;
}


//角度変換　POSから角度へ////////////////////////////////////////////////////////////////////////////////////
/**
* @brief ポジションデータを角度データ(float型)に変換
* @param[in] pos ポジションデータ
* @return 角度(deg)(float型)
* @retval #ANGLE_F_FALSE  正方向範囲外
* @retval -#ANGLE_F_FALSE (0x8000) 負方向範囲外
**/
float KondoClass::posDeg(int pos)
{
  pos = pos - 7500;
  float deg = pos  / 29.633;

  if (deg > MAX_DEG) {
    return ANGLE_F_FALSE;
  }
  if (deg < MIN_DEG) {
    return -ANGLE_F_FALSE;
  }

  return deg;
}


//角度変換　x100 角度からPOSへ///////////////////////////////////////////////////////////////////////////////
/**
* @brief 角度データx100(int型)をポジションデータに変換
* @param[in] deg 角度(deg x100)(int型)
* @return 変換されたポジションデータ
* @retval -1 範囲外
**/
int KondoClass::degPos100(int deg)
{
  if (deg > MAX_100DEG) {
    return -1 ;
  }
  if (deg < MIN_100DEG) {
    return -1;
  }
  long a  = ((long)deg * 2963) / 10000;
  int pos = a + 7500;
  return pos;
}


//角度変換　x100 POSから角度へ///////////////////////////////////////////////////////////////////////////////
/**
* @brief ポジションデータを角度データ(int型)に変換
* @param[in] pos ポジションデータ
* @return 角度(deg x100)(int型)
* @retval #ANGLE_I_FALSE 正方向範囲外
* @retval -#ANGLE_I_FALSE 負方向範囲外
**/
int KondoClass::posDeg100(int pos)
{
  long a = pos - 7500;
  int deg = (a * 1000) / 296;

  if (deg > MAX_100DEG) {
    return ANGLE_I_FALSE;
  }
  if (deg < MIN_100DEG) {
    return -ANGLE_I_FALSE;
  }
  return deg;
}
