
#include "BluetoothSerial.h"
#include <WiFi.h>
#include "EEPROM.h"
#include "driver/rtc_io.h"

#include <math.h>

#define IS_LEFT

#ifdef IS_LEFT
#define ESP_NAME "HaptixModule-Left"
#else
#define ESP_NAME "HaptixModule-Right"
#endif

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

uint32_t _inactive_sleep_timeout = 0;

char inputBuffer[64];
int inputBufferIndex = 0;
char* values[10];

int _adc = 0;
int ADC_MAP[50];

bool _stopped = true;
int start_time_inactive = 0;

RTC_DATA_ATTR int bootCount = 0;

EEPROMClass  INACTIVE_TIMEOUT("eeprom0", 0x100);

#define PWM_FREQ 16000
#define PWM_RESOLUTION 8
#define USE_SIGMADELTA

#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)

void setup_PWM(int pin)
{
  pinMode(pin, OUTPUT);
#ifdef USE_SIGMADELTA
  sigmaDeltaSetup(_adc, PWM_FREQ);
  sigmaDeltaAttachPin(pin, _adc);
#else

  ledcSetup(_adc, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(pin, _adc);
#endif
  ADC_MAP[pin] = _adc;

  _adc = _adc + 1;
}

void pwm_write(int pin, uint8_t val)
{
#ifndef USE_SIGMADELTA
  ledcWrite(ADC_MAP[pin], val);
#else
  sigmaDeltaWrite(ADC_MAP[pin], val);
#endif
}

class Actuator
{
    int _pwm;
    int _dir;
    float _limit;
  public:
    Actuator(int pwm, int dir): _pwm(pwm), _dir(dir)
    {
      _limit=1;
      setup_PWM(pwm);
      pinMode(dir, OUTPUT);

      SetSpeed(0,1);
    }

    void SetSpeed(int speed,float limit)
    {
      _limit=limit;
      if (speed > 0)
      {
        digitalWrite(_dir, LOW);
        pwm_write(_pwm, min(255, (int)(speed*_limit)));
      } else
      {

        digitalWrite(_dir, HIGH);
        pwm_write(_pwm, min(255, -(int)(speed*_limit)));
      }
    }
};

#ifdef IS_LEFT
Actuator _actuators[4] = {
  Actuator(26, 18),
  Actuator(32, 17),
  Actuator(25, 21),
  Actuator(33, 19)
};
#else

Actuator _actuators[4] = {
  Actuator(25, 21),
  Actuator(32, 17),
  Actuator(26, 18),
  Actuator(33, 19)
};
#endif


BluetoothSerial SerialBT;

#define LED_BUILTIN 22
//#define LED_BUILTIN 2
void Blink(int count, int d)
{

  for (int i = 0; i < count; ++i)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(d);
    digitalWrite(LED_BUILTIN, LOW);
    delay(d * 2);
  }
}

int input_vals[4];
void ProcessInput()
{
  int sum=0;
  float limiter=0;
  switch (values[0][0]) {
    case 'd'://drive
      for (int i = 0; i < 4; ++i) {
        int pos = atoi(values[i + 1]);
        input_vals[i]=pos;
        sum+=abs(pos);
      }
      limiter=1;
      if(sum>0)
      {
        limiter=(float)min(sum,700.0f)/(float)sum;
      }
      for (int i = 0; i < 4; ++i) {
        _actuators[i].SetSpeed(input_vals[i],1);
      }
      
      _stopped = false;
      break;
    case 'q'://quit
      for (int i = 0; i < 4; ++i) {
        _actuators[i].SetSpeed(0,1);
      }
      if (!_stopped)
      {
        _stopped = true;
        start_time_inactive = millis();
      }
      break;
    case 't'://Timeout

      _inactive_sleep_timeout = atoi(values[1]);
      SerialBT.println(_inactive_sleep_timeout);
      INACTIVE_TIMEOUT.put(0, _inactive_sleep_timeout);
      _inactive_sleep_timeout = _inactive_sleep_timeout * 1000;
      break;
    case 'v'://version

      SerialBT.print("1.0.0");
      break;
  }
}


void ProcessSerial()
{
  while (SerialBT.available() > 0)
  {
    char c = SerialBT.read();
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

      ProcessInput();

    } else
    {
      inputBuffer[inputBufferIndex++] = c;
    }
  }
}

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("Wakeup caused by timer"); break;
    case 4  : Serial.println("Wakeup caused by touchpad"); break;
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}

void _setup()
{
}


void setup()
{
  Serial.begin(115200);
  if (!INACTIVE_TIMEOUT.begin(INACTIVE_TIMEOUT.length())) {
    //delay(1000);
    //ESP.restart();
  }

  INACTIVE_TIMEOUT.get(0, _inactive_sleep_timeout);
  if (_inactive_sleep_timeout == 0)
  {
    _inactive_sleep_timeout = 5*60; //5*60;//5mins
    INACTIVE_TIMEOUT.put(0, _inactive_sleep_timeout);
  }
  Serial.println();
  Serial.print("Sleep Timeout:  ");
  Serial.println(_inactive_sleep_timeout);
  _inactive_sleep_timeout = _inactive_sleep_timeout * 1000; //convert to milliseconds

  //Increment boot number and print it every reboot
  ++bootCount;
  WiFi.mode( WIFI_MODE_NULL );
  Serial.println("Boot number: " + String(bootCount));


  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  pinMode(26, INPUT_PULLDOWN);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_26, 1); //1 = High, 0 = Low


  _setup();
  _stopped = true;
  start_time_inactive = millis();
  SerialBT.begin(ESP_NAME); //Bluetooth device name

  pinMode(2, OUTPUT);

  Blink(3, 100);
}

void loop()
{
  ProcessSerial();

  if (_stopped && (millis() - start_time_inactive) > _inactive_sleep_timeout)
  {
    Serial.println(millis() - start_time_inactive);
    Serial.println("Going to sleep now");

    Blink(5, 50);
    esp_deep_sleep_start();
  }
  delay(60);

}
