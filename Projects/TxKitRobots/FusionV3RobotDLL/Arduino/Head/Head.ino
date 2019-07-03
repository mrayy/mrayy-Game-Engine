//IMPORTANT: 
// Board type: SparkFun Pro Micro 
// Processor: 5V, 16MHz
//If board bricked, shorten GND and RST twice and quickly reupload empty sketch (only 8 seconds available)


// Updated --> Board ESP32

#include "Kondo.h"
#include "MPU6050_tockn.h"
#include <Wire.h>
#include "imumaths.h"
#include "BluetoothSerial.h"

#define ESP_NAME "HeadBT"

#define TargetSerial SerialBT

BluetoothSerial SerialBT;


MPU6050 mpu6050(Wire);
KondoClass Kondo;

char inputBuffer[64];
int inputBufferIndex=0;

Quaternion inputQuat;
Quaternion IMUQuat;
Quaternion appliedQuat;

Stream*targetStream=&Serial;
Vector<3> eulerAngles;
char* values[10];
float angles[3];
float realAngles[3];

bool isFree=true;
bool debugAng=false;
bool stabilizer=true;

float lastYaw;
float yawErr=0;
float yawDamping=0.99;

#define ENABLE_DEBUG false

unsigned long currenttime,lasttime;
unsigned long reportMillis=30;

class FPSCounter
{
  int lastTime;
  int frame;
  int FPS;

  public:
  void begin()
  {
    lastTime=millis();
    frame=0;
  }
  int getFPS()
  {
    return FPS;
  }
  bool update()
  {
    int current=millis();
    frame++;
    if(current-lastTime>1000)
    {
      FPS=frame;
      frame=0;
      lastTime=current;
      return true;
    }
    return false;
  }
};

FPSCounter fps;
void SetHeadAngles(float tilt,float yaw,float roll)
{
  if(isFree)
    return;

  float v;
  v=Kondo.setAngle(2,tilt);if(v!=ICS_FALSE && abs(v)<200)realAngles[0]=(v);
  v=Kondo.setAngle(1,yaw);if(v!=ICS_FALSE && abs(v)<200)realAngles[1]=(v);//else TargetSerial.println(v);
  v=Kondo.setAngle(3,roll);if(v!=ICS_FALSE && abs(v)<200)realAngles[2]=(v);
  if(debugAng && abs(currenttime-lasttime)>reportMillis)
  {
    lasttime=currenttime;
    String str="@ang ";
    str+=(realAngles[0]-mpu6050.getAngleX());
    str+=(",");
    str+=(realAngles[1]-yawErr);
    str+=(",");
    str+=(realAngles[2]-mpu6050.getAngleY());
    str+="#";
    targetStream->println(str);
  }
}
//@0,90,0#
void applyRotation()
{
  if(stabilizer)
    appliedQuat=IMUQuat*inputQuat;
  else
    appliedQuat=inputQuat;

  eulerAngles=Quaternion::toEuler(appliedQuat,Quaternion::xzy);
  /*if(isDebug)
  {
    Serial.print(eulerAngles.z()); Serial.print(" ");
    Serial.print(eulerAngles.x());Serial.print(" ");
    Serial.print(eulerAngles.y());Serial.println();
  }*/
  SetHeadAngles(eulerAngles.z(),eulerAngles.x(),eulerAngles.y());
}

void setup()
{
  Serial.begin(115200);
  SerialBT.begin(ESP_NAME); //Bluetooth device name
  Kondo.begin(625000,&Serial2); // 115200,625000,1250000
  Kondo.setFree(1);
  Kondo.setFree(2);
  Kondo.setFree(3);

  Wire.begin(21,22,400000);
  //Wire.setSpeed(1);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(ENABLE_DEBUG);

  currenttime=lasttime=millis();
  fps.begin();
}

void ProcessInput(int paramsN,Stream* srcStream)
{
  //TargetSerial.print(paramsN);
   /* Serial.print(angles[0]);Serial.print(" ");
    Serial.print(angles[1]);Serial.print(" ");
    Serial.print(angles[2]);Serial.println(" ");*/
    switch(values[0][0]){
      case 'd':
      isFree=false;
      if(paramsN<3)
        break;
      angles[0]=atof(values[1])/100.0f;
      angles[1]=atof(values[2])/100.0f;
      angles[2]=atof(values[3])/100.0f;
     inputQuat=Quaternion(angles[0],Vector<3>(1,0,0))*Quaternion(angles[1],Vector<3>(0,1,0))*Quaternion(angles[2],Vector<3>(0,0,1));
     applyRotation();
     break;
     case 'e':
      if(values[0][1]=='a')
      {
        debugAng=true;
        targetStream=srcStream;
        if(paramsN<1)
          break;
        reportMillis=atoi(values[1]);
      }else
      if(values[0][1]=='s')
      {
        stabilizer=true;
        applyRotation();
      }
     break;
     case 'y'://yaw damping
      yawDamping=atof(values[1]);
     break;
     case 's':
      if(values[0][1]=='a')
      {
        debugAng=false;
      }else
      if(values[0][1]=='s')
      {
        stabilizer=false;
        applyRotation();
      }
     break;
     case 'q':
       isFree=true;
       Kondo.setFree(1);
       Kondo.setFree(2);
       Kondo.setFree(3);
     break;
    }
}

void ProcessSerial(Stream *s)
{
  while(s->available()>0)
  {
    char c=s->read();
    if(c=='@')
    {
      inputBufferIndex=0;
      //Serial.println("start parsing");
    }else if(c=='#')
    {
      inputBuffer[inputBufferIndex]=0;
      //Serial.print("done parsing: ");
      //Serial.println(inputBuffer);
      char *p = inputBuffer;
      char *str;
      int i=0;
      while ((str = strtok_r(p, ",", &p)) != NULL) // delimiter is the semicolon
        values[i++]=str;

      ProcessInput(i-1,s);

    }else
    {
      inputBuffer[inputBufferIndex++]=c;
    }
  }
}

void loop()
{
  currenttime=millis();
  
  if(stabilizer && !isFree){
    mpu6050.update();

    yawErr=(yawErr+(mpu6050.getAngleZ()-lastYaw))*yawDamping;
    lastYaw=mpu6050.getAngleZ();

    
    IMUQuat.fromEuler(mpu6050.getAngleX(),yawErr,mpu6050.getAngleY());
    applyRotation();
  }
  ProcessSerial(&Serial);
  ProcessSerial(&SerialBT);

  if(fps.update() && ENABLE_DEBUG)
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
