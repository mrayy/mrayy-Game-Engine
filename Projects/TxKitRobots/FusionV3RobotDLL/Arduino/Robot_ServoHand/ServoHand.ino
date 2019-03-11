


#include <Servo.h>


char inputBuffer[64];
int inputBufferIndex=0;
char* values[10];

int _lastTime=0;

#define SENSOR_UPDATE 50

#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)

class Actuator
{
  int _pin;
  Servo _servo; 
  int _pos;

  int _default;
  #define MinVal 40
  #define MaxVal 150
  public:
  Actuator(int pin,int def=10):_pin(pin),_pos(0),_default(def)
  {
    //pinMode(_pin,OUTPUT);
  }

  void Setup()
  {
    
    _servo.attach(_pin);
    Reset();
  }

  void Reset()
  {
    _pos=100-_default;
  }

  void SetPos(int pos)
  {
    if(pos<0)pos=0;
    if(pos>100)pos=100;

    //analogWrite(_pin,pos);
    _pos=100-pos;
    
    _servo.write(MinVal+ _pos*(MaxVal-MinVal)/100.0f+sign*5);
   // sign*=-1;
  }
  int sign=1;
  void Update()
  {
  }
};

class PressureSensor
{
  
  int minValue;
  int maxValue;
  int _pin;
  int _value;
  public:
  PressureSensor(int pin):_pin(pin)
  {}

  void Setup()
  {
   // pinMode(_pin,INPUT);
    Reset();
  }

  void Reset()
  {
    minValue=1024;
    maxValue=0;
  }

  void Update()
  {
    _value=analogRead(_pin);
    
    if(_value<minValue)
      minValue=_value;
      
    if(_value>maxValue)
      maxValue=_value;
  
    if(maxValue>minValue)
      {
        _value=(_value-minValue);
        _value=1024*(float)_value/(float)(maxValue-minValue);
      }
    //Serial.println(_value);
  
  }

  int Value()
  {
    return _value;
  }
};

#define ACTUATORS_COUNT 6
Actuator _actuators[ACTUATORS_COUNT ]={
  Actuator(16),
  Actuator(10,10),//10
  Actuator(9,20),//9 
  Actuator(6,30),//6
  Actuator(5,40),//5
  Actuator(3,50)//3
};

#define SENSORS_COUNT 3
PressureSensor _sensors[SENSORS_COUNT ]={
  PressureSensor(A0),
  PressureSensor(A1),
  PressureSensor(A2)
};

void Blink(int count,int d)
{
  
    for(int i=0;i<count;++i)
    {
      digitalWrite(LED_BUILTIN,HIGH);
      delay(d);
      digitalWrite(LED_BUILTIN,LOW);
      delay(d*2);
    }
}

void ProcessInput()
{
    switch(values[0][0]){
      case 'd'://drive
        for(int i=0;i<ACTUATORS_COUNT ;++i){
          int pos=atoi(values[i+1]);
          _actuators[i].SetPos(pos);
        }
        break;
        case 'q'://quit
        for(int i=0;i<ACTUATORS_COUNT ;++i){
          _actuators[i].SetPos(0);
        }
     break;
        case 't'://Timeout

     break;
        case 'v'://version

        Serial.print("1.0.0");
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



void setup()
{
  Serial.begin(115200);
  Blink(3,100);
  for(int i=0;i<ACTUATORS_COUNT ;++i){
    _actuators[i].Setup();
  }
  for(int i=0;i<SENSORS_COUNT ;++i){
    _sensors[i].Setup();
  }

  _lastTime=millis();
}

void loop()
{
  ProcessSerial();
  for(int i=0;i<ACTUATORS_COUNT ;++i){
    _actuators[i].Update();
  }
#if FALSE
  if(_lastTime-millis()>SENSOR_UPDATE)
  {
    Serial.print("@s,");
    for(int i=0;i<SENSORS_COUNT ;++i){
      _sensors[i].Update();
      Serial.print(_sensors[i].Value());
      if(i!=SENSORS_COUNT-1)
        Serial.print(", ");
    }
    Serial.println("#");
    _lastTime=millis();
  }
#endif
      delay(20);

}
