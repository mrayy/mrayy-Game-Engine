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

bool speedSet=false;


unsigned long currenttime, lasttime,starttime;
int reportMillis=30;

class DTween
{
    float Step(
        float current, float target, float& velocity, float omega,float dt)
    {
        float n1 = velocity - (current - target) * (omega * omega * dt);
        float n2 = 1 + omega * dt;
        velocity = n1 / (n2 * n2);
        return current + velocity * dt;
    }
public:
     float position;
     float velocity;
     float omega;

     DTween(float position, float omega)
    {
        this->position = position;
        this->velocity = 0;
        this->omega = omega;
    }

     float Step(float target,float dt)
    {
        position = Step(position, target, velocity, omega,dt);
        return position;
    }
};

DTween yawAngle(0,1);


void SetHeadAngles(float tilt,float yaw,float roll)
{

  float v;
  v=Kondo.setAngle(2,tilt);if(v!=ICS_FALSE && abs(v)<200)realAngles[2]=(v);
  v=Kondo.setAngle(1,yaw);if(v!=ICS_FALSE && abs(v)<200)realAngles[1]=(v);
  v=Kondo.setAngle(3,roll);if(v!=ICS_FALSE && abs(v)<200)realAngles[0]=(v);
  if(debugAng && abs(currenttime-lasttime)>reportMillis)
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

  Kondo.setSpeed(1,20);
  Kondo.setSpeed(2,20);
  Kondo.setSpeed(3,20);

  Wire.begin();
  //Wire.setSpeed(1);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  currenttime=lasttime=micros();
}

void ProcessInput()
{
   /* Serial.print(angles[0]);Serial.print(" ");
    Serial.print(angles[1]);Serial.print(" ");
    Serial.print(angles[2]);Serial.println(" ");*/
    switch(values[0][0]){
      case 'd':
      if(isFree)
      {
        Kondo.setSpeed(1,20);
        Kondo.setSpeed(2,20);
        Kondo.setSpeed(3,20);
        
        Kondo.setAngle(3, 0);
        Kondo.setAngle(1, 0);
        Kondo.setAngle(2, 0);
        speedSet=false;
        starttime=millis();
      }
      isFree=false;
      if ( !speedSet)
        break;
      angles[2]=atof(values[1]);
      angles[1]=atof(values[2]);
      angles[0]=atof(values[3]);
     inputQuat=Quaternion(angles[0],Vector<3>(1,0,0))*Quaternion(angles[1],Vector<3>(0,1,0))*Quaternion(angles[2],Vector<3>(0,0,1));
     if(!stabilizer)
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
       // applyRotation();
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
       // applyRotation();
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

#define sign(a) ((a>=0)?1:-1)
void loop()
{
  currenttime=millis();
  
  if(stabilizer && !isFree){
    mpu6050.update();
    if(!speedSet && currenttime-starttime>1000)
    {
      Kondo.setSpeed(1,127);
      Kondo.setSpeed(2,127);
      Kondo.setSpeed(3,127);
      speedSet=true;
      
    }
    //eulerAngles.z()=mpu6050.getAngleX();
    //eulerAngles.y()=mpu6050.getAngleY();
    double currYaw = mpu6050.getAngleZ();
    yawAngle.Step(currYaw,0.04f);
    IMUQuat.fromEuler(mpu6050.getAngleX(),currYaw-yawAngle.position,mpu6050.getAngleY());
    applyRotation();
  }
  ProcessSerial();

  //SetHeadAngles(mpu6050.getAngleX(),0,mpu6050.getAngleY());

  /*Serial.print(mpu6050.getAngleX()); Serial.print(" ");
  Serial.print(mpu6050.getAngleZ());Serial.print(" ");
  Serial.print(mpu6050.getAngleY());Serial.println();*/
  //Serial.println(Kondo.setAngle(2,mpu6050.getAngleX()));
  //Serial.println(Kondo.setAngle(1,mpu6050.getAngleZ()));
  //Serial.println(Kondo.setAngle(3,mpu6050.getAngleY()));
  //delay(1);
}
