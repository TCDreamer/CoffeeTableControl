
const int ShiftPWM_latchPin=3;

const bool ShiftPWM_invertOutputs = false; 

const bool ShiftPWM_balanceLoad = false;

#define SHIFTPWM_USE_TIMER3                  //Use Timer3 on Arduino Leonardo to prevent PWM issues on pins 9 and 10.

#include <ShiftPWM.h>                        // include ShiftPWM.h after setting the pins!
#include "led.h"

#define NUMBER_OF_595_CHIPS 2

#define NUM_LEDS   NUMBER_OF_595_CHIPS * 8

  
unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 75;  

unsigned long fadeSpeed = 5;                //How many milliseconds are waited to increase or decrease the output.
unsigned long slowFadeSpeed = 1;            
unsigned long lastUpdate = 0;

unsigned long bLastUpdate = 0;
int fadeValue = 255;




  
//led leds[NUM_LEDS]; 

  
  
void ShiftPWM_Setup(void){
  ShiftPWM.SetAmountOfRegisters(NUMBER_OF_595_CHIPS);

  // SetPinGrouping allows flexibility in LED setup. 
  ShiftPWM.SetPinGrouping(1); //This is the default, but I added here to demonstrate how to use the funtion
  
  ShiftPWM.Start(pwmFrequency,maxBrightness);
  
  for(int i = 0; i < NUM_LEDS; i++)        //Sets all LEDs to off by default until they are turned on
  {
    leds[i].currentLevel = 0;
    leds[i].active = false;
    leds[i].half = false;
    leds[i].stops = 0;
  }
  
}



void Fade_Surface(void){
  unsigned long currentTime = millis();
  
  if(currentTime - lastUpdate >= fadeSpeed)
  {  
    for(int i = 0; i < NUM_LEDS; i++)
    {
      if(leds[i].stops == 2)
      {
        if (leds[i].currentLevel < maxBrightness)
        {
//          Serial.println(leds[i].currentLevel);
          leds[i].currentLevel += fadeSpeed;
          if(leds[i].currentLevel > maxBrightness)
            leds[i].currentLevel = 255;
          ShiftPWM.SetOne(i, leds[i].currentLevel);
        }
        else if(leds[i].currentLevel >= maxBrightness)
        {
          leds[i].stops = 0;
        }
      }
      if(leds[i].stops == 0)
      {
        if(leds[i].currentLevel > 0)
        {
 //         Serial.print(i);Serial.print(" : ");
//          Serial.print(leds[i].currentLevel); Serial.print("    ");
  
          leds[i].currentLevel -= fadeSpeed;
          if(leds[i].currentLevel < 0)
          {
            leds[i].currentLevel = 0;
            leds[i].stops = 0;
          }
//          Serial.println(leds[i].currentLevel);
          ShiftPWM.SetOne(i, leds[i].currentLevel);
        }
      }
      else if(leds[i].stops == 1)
      {
        if (leds[i].currentLevel < maxBrightness / 8)
        {
          leds[i].currentLevel += slowFadeSpeed;
       
          ShiftPWM.SetOne(i, leds[i].currentLevel);
        }
      }
      else if(leds[i].stops == 4)
      {
        if(leds[i].currentLevel > 0)
        {
         // Serial.print(i);Serial.print(" : ");
         // Serial.print(leds[i].currentLevel); Serial.print("    ");
  
          leds[i].currentLevel -= slowFadeSpeed;
          if(leds[i].currentLevel < 0)
          {
            leds[i].currentLevel = 0;
            leds[i].stops = 0;
          }
          Serial.println(leds[i].currentLevel);
          ShiftPWM.SetOne(i, leds[i].currentLevel);
        }
      }
    }
    lastUpdate = currentTime;
  }
 // ShiftPWM.SetAll(150);
}

bool Fade_Backlight()
{
  unsigned long currentTime = millis();
  
  if(currentTime - bLastUpdate >= fadeSpeed)
  {
    fadeValue -= fadeSpeed;
    Serial.println(fadeValue);
    if(fadeValue <= 0)
      {
        digitalWrite(LED_Strip_Pin1, LOW);
        digitalWrite(LED_Strip_Pin2, LOW);
        digitalWrite(LED_Strip_Pin4, LOW);
        //if(fadeValue <= -400)
          return true;
        //else
        //  return false;
      }
      else
      {

        analogWrite(LED_Strip_Pin1, fadeValue); 
    //  analogWrite(LED_Strip_Pin2, fadeValue); 
    //  analogWrite(LED_Strip_Pin3, fadeValue); 
        analogWrite(LED_Strip_Pin4, fadeValue); 
        bLastUpdate = currentTime;
        return false;
        
      }
    }
    else
    {
      return false;
    }


}

bool Return_Backlight()
{
  unsigned long currentTime = millis();
  
  if(currentTime - bLastUpdate >= fadeSpeed)
  {
    fadeValue += fadeSpeed;
    Serial.println(fadeValue);
    
    if(fadeValue >= 255)
      {
        digitalWrite(LED_Strip_Pin1, HIGH);

        digitalWrite(LED_Strip_Pin4, HIGH);
        return true;

      }
      else if(fadeValue <= 0)
      {
        digitalWrite(LED_Strip_Pin1, LOW);
        digitalWrite(LED_Strip_Pin4, LOW);
        bLastUpdate = currentTime;
        return false;
      }
      else
      {
        analogWrite(LED_Strip_Pin1, fadeValue); 
    //  analogWrite(LED_Strip_Pin2, fadeValue); 
    //  analogWrite(LED_Strip_Pin3, fadeValue); 
        analogWrite(LED_Strip_Pin4, fadeValue); 
        bLastUpdate = currentTime;
        return false;
      }
    }
    else
    {
      return false;
    }


}

  
