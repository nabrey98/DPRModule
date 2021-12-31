#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
unsigned long counter;
const byte sensor = A0;
bool holdSensor;
unsigned long curTime, oldTime;
unsigned long getTime =60000;
int bufPPM;


void setup()
{
  while (!Serial) {  ;  }
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.clear();
}


void loop()
{ 
//Program Counter  
  if (bacaSensor() == 1)
  {
    if (holdSensor == 0)
    {
      counter++;
      holdSensor = 1;
      
      lcd.setCursor(4,1);
      lcd.println(String (counter)+ ("  "));

    }
  }
  else
  {
    holdSensor = 0;
  }
  hitungPPM(); // value as float
  
}

bool bacaSensor()
{
  bool dtSensor;
  int adc0 = analogRead(sensor);
  if (adc0 > 1000)
  {
    dtSensor = 1;
  }
  else
  {
    dtSensor = 0;
  }
  return dtSensor;
}

float hitungPPM()
{

  curTime = millis();
  if (curTime - oldTime >= getTime)
  {
    bufPPM = counter;
    counter = 0;
    //Serial.print("PPM:");
    Serial.println(bufPPM);
    
    oldTime = curTime;
    
  }
  return bufPPM;
}
