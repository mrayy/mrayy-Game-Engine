#include <SoftwareSerial.h>   // We need this even if we're not using a SoftwareSerial object
#include <Thread.h>
#include <SerialCommand.h>
#define DEFAULT_AVERAGE 10
#include <RunningAverage.h>

float num=567;
float beta=3889;
float rZero=10000;
float v=0;
float newRes=0;
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

#define SensorsCount 4
float temprature[SensorsCount];
RunningAverage average[SensorsCount];

Thread serialThread = Thread();


SerialCommand SCmd;   // The demo SerialCommand object
int delayTime = 30;


void UpdateSerial()
{
  SCmd.readSerial();     // We don't do much, just process serial commands
}
void setup()
{
  Serial.begin(115200);

  // Setup callbacks for SerialCommand commands
  SCmd.addCommand("delay", SetDelay);      

  SCmd.addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?")


  serialThread.onRun(UpdateSerial);
  serialThread.setInterval(500);


}

void Update()
{


  for(int i=0;i<SensorsCount;++i){
    // read the analog in value:
    sensorValue = analogRead(analogInPin+i);
    // map it to the range of the analog out:
    outputValue = map(sensorValue, 0, 1023, 0, 255);
    // change the analog out value:
    average[i].addValue(temperatureCalc(sensorValue));
    temprature[i]=average[i].getAverage();//temperatureCalc(sensorValue);
  }
}
void WriteData() {
    Serial.print("@");
  for(int i=0;i<SensorsCount;++i){
    Serial.print(temprature[i]);
    if(i<SensorsCount-1)
      Serial.print(",");
  }
    Serial.print("#");
}

void loop()
{
  if (serialThread.shouldRun())
    serialThread.run();

  Update();
  WriteData();
  if(delayTime>0)
    delay(delayTime);
}

void SetDelay()
{
  char *arg;

  arg = SCmd.next();
  if (arg != NULL)
  {
    delayTime = atoi(arg);  // Converts a char string to an integer

  }
  else {
  }

}

void unrecognized()
{
}

float temperatureCalc(float t)
{
  v = t * 5.0f / 1023.0f;
  newRes = ( (5.0f / v ) - 1 ) * rZero;
  return (beta/log(newRes/(rZero*exp(-beta/298.0f))))-273.0f;
}

