
//IMPORTANT:
// Board type: SparkFun Pro Micro
// Processor: 5V, 16MHz
//If board bricked, shorten GND and RST twice and quickly reupload empty sketch (only 8 seconds available)

#define ENABLE_DEBUG true

#include "Kondo.h"
#include "MPU6050_tockn.h"
#include <Wire.h>
#include "imumaths.h"
#include "BluetoothSerial.h"

#include "analogWrite.h"

#define ESP_NAME "HeadBT"

#define TargetSerial Serial

BluetoothSerial SerialBT;


MPU6050 mpu6050(Wire);
KondoClass Kondo;

char inputBuffer[64];
int inputBufferIndex = 0;

Quaternion inputQuat;
Quaternion IMUQuat;
Quaternion appliedQuat;
Quaternion transformQuat;

Stream*targetStream = &Serial;
Vector<3> eulerAngles;
char* values[10];
float angles[3];
float IMUAngles[3];
float realAngles[3];

bool isFree = true;
bool debugAng = false;
bool stabilizer = true;

float lastYaw;
float yawErr = 0;
float yawDamping = 0.99;

#define LASER_PIN 26

int laserValue;

unsigned long currenttime, lasttime,starttime;
unsigned long reportMillis = 30;

bool speedSet=false;

float clamp(float x,float a,float b)
{
  if(x<a)return a;
  if(x>b)return b;
  return x;
}

class FPSCounter
{
    int lastTime;
    int frame;
    int FPS;

  public:
    void begin()
    {
      lastTime = millis();
      frame = 0;
    }
    int getFPS()
    {
      return FPS;
    }
    bool update()
    {
      int current = millis();
      frame++;
      if (current - lastTime > 1000)
      {
        FPS = frame;
        frame = 0;
        lastTime = current;
        return true;
      }
      return false;
    }
};

FPSCounter fps;
void SetHeadAngles(float tilt, float yaw, float roll)
{
  if (isFree)
    return;
/*  Serial.print(tilt);
  Serial.print(", ");
  Serial.print(yaw);
  Serial.print(", ");
  Serial.println(roll);*/
  float v;
  v = Kondo.setAngle(1, yaw);// if (v != ICS_FALSE && abs(v) < 200)realAngles[1] = (v); //else TargetSerial.println(v);
  v = Kondo.setAngle(2, roll);// if (v != ICS_FALSE && abs(v) < 200)realAngles[2] = (v);
  v = Kondo.setAngle(3, tilt); //if (v != ICS_FALSE && abs(v) < 200)realAngles[0] = (v);
  if (debugAng && abs(currenttime - lasttime) > reportMillis)
  {
 //   realAngles[1] =Kondo.getAngle(1);
  //  realAngles[2] =Kondo.getAngle(2);
   // realAngles[0] =Kondo.getAngle(3);
    lasttime = currenttime;
    String str = "@ang ";
    str += (realAngles[0] - mpu6050.getAngleX());
    str += (",");
    str += (realAngles[1] - yawErr);
    str += (",");
    str += (realAngles[2] - mpu6050.getAngleY());
    str += "#";
    targetStream->println(str);
  }
}
//@0,90,0#
void applyRotation()
{
  if (stabilizer)
    appliedQuat = IMUQuat  *  inputQuat;
  else
    appliedQuat = inputQuat;

  eulerAngles = Quaternion::toEuler(appliedQuat, Quaternion::xyz);
  /*if(isDebug)
    {
    Serial.print(eulerAngles.z()); Serial.print(" ");
    Serial.print(eulerAngles.x());Serial.print(" ");
    Serial.print(eulerAngles.y());Serial.println();
    }*/
  SetHeadAngles(eulerAngles.z(), eulerAngles.y(), eulerAngles.x());
}

void setup()
{
  Serial.begin(115200);
  SerialBT.begin(ESP_NAME); //Bluetooth device name
  Kondo.begin(115200,18, &Serial2); // 115200,625000,1250000
  Kondo.setFree(1);
  Kondo.setFree(2);
  Kondo.setFree(3);

  Wire.begin(21, 22, 400000);
  Kondo.setSpeed(1,20);
  Kondo.setSpeed(2,20);
  Kondo.setSpeed(3,20);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(ENABLE_DEBUG);

  currenttime = lasttime = millis();
  fps.begin();

  pinMode(LASER_PIN, OUTPUT);

  transformQuat.fromEuler(180, 0, 0);
}

void ProcessInput(int paramsN, Stream* srcStream)
{
  //TargetSerial.print(paramsN);
  /* Serial.print(angles[0]);Serial.print(" ");
    Serial.print(angles[1]);Serial.print(" ");
    Serial.print(angles[2]);Serial.println(" ");*/
  switch (values[0][0]) {
    case 'd':
      if(isFree)
      {
        Kondo.setAngle(3, 0);
        Kondo.setAngle(1, 0);
        Kondo.setAngle(2, 0);
        speedSet=false;
        starttime=millis();
      }
      isFree = false;
      if (paramsN < 3 || !speedSet)
        break;
      angles[0] = atof(values[1]);
      angles[1] = clamp(atof(values[2]),-80,80);
      angles[2] = atof(values[3]);
      inputQuat = Quaternion(angles[0], Vector<3>(1, 0, 0)) * Quaternion(angles[1], Vector<3>(0, 1, 0)) * Quaternion(angles[2], Vector<3>(0, 0, 1));//.fromEuler(angles[0],angles[1],angles[2]);//
      if(!stabilizer)
        applyRotation();
      break;
    case 'e':
      if (values[0][1] == 'a')
      {
        debugAng = true;
        targetStream = srcStream;
        if (paramsN < 1)
          break;
        reportMillis = atoi(values[1]);
      } else if (values[0][1] == 's')
      {
        stabilizer = true;
        applyRotation();
      }
      break;
    case 'y'://yaw damping
      yawDamping = atof(values[1]);
      yawDamping = max(min(yawDamping, 1.0f), 0.0f);
      break;
    case 'l'://laser
      {
        laserValue = map(atoi(values[1]), 0, 100, 0, 255);

      }
      break;
    case 's':
      if (values[0][1] == 'a')
      {
        debugAng = false;
      } else if (values[0][1] == 's')
      {
        stabilizer = false;
        applyRotation();
      }
      break;
    case 'q':
      isFree = true;
      Kondo.setSpeed(1,20);
      delay(20);
      Kondo.setSpeed(2,20);
      delay(20);
      Kondo.setSpeed(3,20);
      delay(100);
      
      Kondo.setAngle(3, -70);
      Kondo.setAngle(1, 0);
      Kondo.setAngle(2, 0);

      delay(1000);
      Kondo.setFree(1);
      Kondo.setFree(2);
      Kondo.setFree(3);
      
      speedSet=false;
      break;
    case 'v':
    Serial.println("Head Module 3.0");
    
    break;
  }
}

void ProcessSerial(Stream *s)
{
  while (s->available() > 0)
  {
    char c = s->read();
    if (c == '@')
    {
      inputBufferIndex = 0;
      //Serial.println("start parsing");
    } else if (c == '#')
    {
      inputBuffer[inputBufferIndex] = 0;
      //Serial.print("done parsing: ");
      //Serial.println(inputBuffer);
      char *p = inputBuffer;
      char *str;
      int i = 0;
      while ((str = strtok_r(p, ",", &p)) != NULL) // delimiter is the semicolon
        values[i++] = str;

      ProcessInput(i - 1, s);

    } else
    {
      inputBuffer[inputBufferIndex++] = c;
    }
  }
}

void loop()
{
  currenttime = millis();

  if (stabilizer && !isFree) {
    mpu6050.update();

    if(!speedSet && currenttime-starttime>1000)
    {
      Kondo.setSpeed(1,127);
      delay(20);
      Kondo.setSpeed(2,127);
      delay(20);
      Kondo.setSpeed(3,127);
      delay(20);
      speedSet=true;
      
    }

    yawErr = (yawErr + (mpu6050.getAngleZ() - lastYaw)) * yawDamping;
    lastYaw = mpu6050.getAngleZ();
    /*
        Serial.print(mpu6050.getAngleX());
        Serial.print(",");
        Serial.print(mpu6050.getAngleY());
        Serial.print(",");
        Serial.println(mpu6050.getAngleZ());*/
    IMUQuat.fromEuler(-mpu6050.getAngleX(), yawErr, mpu6050.getAngleY());
    IMUAngles[0]=-mpu6050.getAngleX();
    IMUAngles[1]=yawErr;
    IMUAngles[2]=mpu6050.getAngleY();
    //Serial.print(IMUQuat.x());Serial.print(",");
    //Serial.print(IMUQuat.y());Serial.print(",");
    //Serial.print(IMUQuat.z());Serial.print(",");
    //Serial.println(IMUQuat.w());
    //IMUQuat=transformQuat*IMUQuat;
    applyRotation();
  }
  ProcessSerial(&Serial);
  ProcessSerial(&SerialBT);

  if (angles[0] > 0 || isFree)
    analogWrite(LASER_PIN, 0);
  else
    analogWrite(LASER_PIN, laserValue);

  if (fps.update() && ENABLE_DEBUG)
  {
    Serial.println(fps.getFPS());
  }
  //SetHeadAngles(mpu6050.getAngleX(),0,mpu6050.getAngleY());

  /*Serial.print(mpu6050.getAngleX()); Serial.print(" ");
    Serial.print(mpu6050.getAngleZ());Serial.print(" ");
    Serial.print(mpu6050.getAngleY());Serial.println();*/
  //Serial.println(Kondo.setAngle(2,mpu6050.getAngleX()));
  //Serial.println(Kondo.setAngle(1,mpu6050.getAngleZ()));
  //Serial.println(Kondo.setAngle(3,mpu6050.getAngleY()));
  //delay(1);
}
