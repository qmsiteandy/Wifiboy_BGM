#include <wifiboypro.h>
#include <Arduino.h>

int pitch[] = {440,500,800,440}; //音調
int tempo[] = {1000,2000,1000,1000};  //節奏(1000=1秒)
int volume = 5; //音量

float soundEndTime = 0; //儲存音調結束時間
int pitch_counter = 0;  //現在播放第幾個音

void setup_sound()
{
  pinMode(17, OUTPUT); 
  digitalWrite(17,1);
  pinMode(25, OUTPUT);
  ledcSetup(1, 400, 8); // ledc/pwm ch#1, freq=400, duty resolution=8bit (0-255)
  ledcAttachPin(25, 1); // IO#25=buzzer, attached to pwm#1
}

void make_sound(int _freq, int _volumn, float _timeLength)
{
  ledcSetup(1, _freq, 8);
  ledcWrite(1, _volumn);
  soundEndTime = millis() + _timeLength;
}

void setup() 
{
  wbpro_init(); // init WiFiBoy32 engine
  wbpro_initBuf8(); // prepare 76800-byte off-screen buffer

  setup_sound(); //初始化Buzzer
}

void loop() 
{
  wbpro_clearBuf8(); // clear off-screen buffer
  

  //如果譜還沒結束，並且上一個音已經播完，換下一個音
  if((pitch_counter < (sizeof(pitch)/sizeof(pitch[0]))) && (millis() >= soundEndTime))
  {
    make_sound(pitch[pitch_counter], volume, tempo[pitch_counter]);
    pitch_counter += 1;
  }
  //如果譜已經播完，並且上一個音已經結束，停止Buzzer輸出
  else if((pitch_counter >= (sizeof(pitch)/sizeof(pitch[0])) && (millis() >= soundEndTime)))
  {
      ledcWrite(1, 0);  //停止音樂
  }


  wbpro_blit8(); // blit to LCD
}
