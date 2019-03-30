
//Written by: MHD Yamen Saraiji
//IMPORTANT: 
// Board type: SparkFun Pro Micro 
// Processor: 5V, 16MHz
//If board bricked, shorten GND and RST twice and quickly reupload empty sketch (only 8 seconds available)

#include "Kondo.h"
#include "MPU6050_tockn.h"
#include <Wire.h>
#include "imumaths.h"

MPU6050 mpu6050(Wire);
KondoClass Kondo;

char inputBuffer[64];
int inputBufferIndex=0;

Quaternion inputQuat;
Quaternion IMUQuat;
Quaternion appliedQuat;

Vector<3> eulerAngles;
char* values[10];
float angles[3];
float realAngles[3];

bool isFree=true;
bool debugAng=false;
bool stabilizer=true;

unsigned long currenttime,lasttime;

void SetHeadAngles(float tilt,float yaw,float roll)
{
  if(isFree)
    return;
  tilt=min(40,max(-40,tilt));
  yaw=min(90,max(-90,yaw));
  roll=min(40,max(-40,roll));
  float v;
  v=Kondo.setAngle(2,tilt);if(v!=ICS_FALSE && abs(v)<200)realAngles[0]=(v);
  v=Kondo.setAngle(1,yaw);if(v!=ICS_FALSE && abs(v)<200)realAngles[1]=(v);
  v=Kondo.setAngle(3,roll);if(v!=ICS_FALSE && abs(v)<200)realAngles[2]=(v);
  if(debugAng && abs(currenttime-lasttime)>16000)
  {
    lasttime=currenttime;
    Serial.print("@ang ");
    Serial.print(realAngles[0]-mpu6050.getAngleX());Serial.print(",");
    Serial.print(realAngles[1]);Serial.print(",");
    Serial.print(realAngles[2]-mpu6050.getAngleY());Serial.println("#");
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
  Kondo.begin(115200,&Serial1);
  Kondo.setFree(1);
  Kondo.setFree(2);
  Kondo.setFree(3);

  Wire.begin();
  //Wire.setSpeed(1);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  currenttime=lasttime=micros();

  //isFree=false;
  //SetHeadAngles(40,100,140);
}

void ProcessInput()
{
   /* Serial.print(angles[0]);Serial.print(" ");
    Serial.print(angles[1]);Serial.print(" ");
    Serial.print(angles[2]);Serial.println(" ");*/
    switch(values[0][0]){
      case 'd':
      isFree=false;
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
      }else
      if(values[0][1]=='s')
      {
        stabilizer=true;
        applyRotation();
      }
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

void ProcessSerial()
{
  while(Serial.available()>0)
  {
    char c=Serial.read();
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

      ProcessInput();

    }else
    {
      inputBuffer[inputBufferIndex++]=c;
    }
  }
}

void loop()
{
  currenttime=micros();
  
  if(stabilizer){
    mpu6050.update();
    eulerAngles.z()=mpu6050.getAngleX();
    eulerAngles.y()=mpu6050.getAngleY();
    applyRotation();
  }
  ProcessSerial();
  IMUQuat.fromEuler(mpu6050.getAngleX(),0,mpu6050.getAngleY());

  //SetHeadAngles(mpu6050.getAngleX(),0,mpu6050.getAngleY());

  /*Serial.print(mpu6050.getAngleX()); Serial.print(" ");
  Serial.print(mpu6050.getAngleZ());Serial.print(" ");
  Serial.print(mpu6050.getAngleY());Serial.println();*/
  //Serial.println(Kondo.setAngle(2,mpu6050.getAngleX()));
  //Serial.println(Kondo.setAngle(1,mpu6050.getAngleZ()));
  //Serial.println(Kondo.setAngle(3,mpu6050.getAngleY()));
  //delay(1);
}
