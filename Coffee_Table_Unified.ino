/*
Coffee Table Control

-Controls the following:
  -Backlighting based off of Ambient Light
  -Backlighting based on Computer Power State / User Preference
  -Touch Activation on/off in direct IR light
  -Capacitive Home Button(s)
  -Capacitive Rotate Button Combinations
  -Surface LED indicators for Capacitive Buttons
 
 
 */
#include "led.h"

////Backlight Definitions
int LED_Strip_Pin1 =  6;    // LED connected to digital pin 6. Lower Hard Strips
int LED_Strip_Pin2 =  9;    // LED connected to digital pin 9. Rim Strip
int LED_Strip_Pin3 = 10;    // LED connected to digital pin 10. Bottom Panels
int LED_Strip_Pin4 = 11;    // LED connected to digital pin 11. Upper Hard Strips

int LED_Bright_Level = 100;
int LED_Low_Level = 50;
int LED_Off = 0;

unsigned long lastBackLightUpdate = 0;



extern bool buttonPressed[16];
extern int fadeValue;

s_led leds[16]; 

buttonGroup bGroup[2];

unsigned int pinValues;
unsigned int oldPinValues;

int ledxref[16] = {99, 13, 12, 11, 10, 9, 99, 99, 99, 1, 2, 3, 4, 5, 6, 7};

int flipTrigger = 0;





void setup()  { 
  //Set Pins connected to LED strips as Outputs
    pinMode(LED_Strip_Pin1, OUTPUT);
    pinMode(LED_Strip_Pin2, OUTPUT);
    pinMode(LED_Strip_Pin3, OUTPUT);
    pinMode(LED_Strip_Pin4, OUTPUT);
    
    digitalWrite(LED_Strip_Pin1, HIGH);
    digitalWrite(LED_Strip_Pin2, LOW);
    digitalWrite(LED_Strip_Pin3, LOW);
    digitalWrite(LED_Strip_Pin4, HIGH);
  
 
  //Setup for the ShiftPWM library and the pins needed to fade the surface LEDs
    pinMode(12, OUTPUT);      //Set Enable Output pin as Output
    pinMode( 2, OUTPUT);      //Set Cap/LED reset pin to Output. Controls GND to Registers to enable resets.
    
    digitalWrite(12, HIGH);   //Disables Output of the 565 Shift Registers until setup is complete
    digitalWrite( 2, HIGH);   //Turns on Shift Registers

    ShiftPWM_Setup();
  
  //Setup for shift in from 74HC165 Registers to determine what Surface Buttons have been triggered
    CapSense_Setup();
  
    bGroupInit();
  
  
  //Enable Keyboard Output
    Keyboard.begin();
    
  //Enable Serial Communication
    Serial.begin(9600);
  


} 

void loop()  { 
 digitalWrite( 12, LOW); 
 // sensorValue = analogRead(sensorPin);
 // Serial.print(sensorValue); Serial.print("  :  ");
  //float scale = sensorValue / 1023;
 // Serial.print(scale); Serial.print("  :  ");
 
 
/* for(int y = 0; y<256; y++)
 {
   analogWrite(LED_Strip_Pin1, y); 
   analogWrite(LED_Strip_Pin2, y); 
   analogWrite(LED_Strip_Pin3, y); 
   analogWrite(LED_Strip_Pin4, y); 
   delay(10);
 }
 
  for(int y = 255; y>=0; y--)
 {
   analogWrite(LED_Strip_Pin1, y); 
   analogWrite(LED_Strip_Pin3, y); 
   analogWrite(LED_Strip_Pin2, y); 
   analogWrite(LED_Strip_Pin4, y); 
   delay(10);
 }*/
 
/*  digitalWrite(LED_Strip_Pin1, HIGH);
  digitalWrite(LED_Strip_Pin2, LOW);
  digitalWrite(LED_Strip_Pin3, LOW);
  digitalWrite(LED_Strip_Pin4, HIGH);*/

    pinValues = read_shift_regs();

     /* If there was a chage in state, display which ones changed.
     */
     if(pinValues != oldPinValues)
     {
      //   Serial.print("*Pin value change detected*\r\n");
         display_pin_values();
         oldPinValues = pinValues;
     }

     delay(1);


  
  /*for(int p = 0; p < 16; p++)
  {
    if(ledxref[p] != 99)
    {

      if(buttonPressed[p] != leds[ledxref[p]].active)
      {
              Serial.print(p);Serial.print(" ; ");Serial.print(ledxref[p]);Serial.println();
          leds[ledxref[p]].active = buttonPressed[p];
      }
    }
  }*/

 CheckStart();
 gCheck3();
 
 Fade_Surface(); 
}


void CheckStart()
{
  static int start1 = 0;
  static int start2 = 0;
  
  
  if(buttonPressed[9] == true && start1 == 0)
  {
    Start_Pressed('F');
    start1 = 1;
    leds[ledxref[9]].stops = 1;
  }
  if(buttonPressed[15] == true && start2 == 0)
  {
    Start_Pressed('R');
    start2 = 1;
    leds[ledxref[15]].stops = 1;
  }
  if((buttonPressed[9] == false && start1 == 1))
  {
    Start_Released('F');
    leds[ledxref[9]].stops = 2;
    
    start1 = 3;
  }
  if((buttonPressed[15] == false && start2 == 1))
  {
    Start_Released('R');
    leds[ledxref[15]].stops = 2;
    
    start2 = 3;
  }
  
  if(start1 == 3 || start2 == 3)
  {
    if(fadeValue <= 0)                    //If the backlight is off, return it to being on.
    {
      while(Return_Backlight() != true)
        Serial.println(fadeValue);
    }
    start1 = 0;
    start2 = 0;
  }
      
    
}


void gCheck3()
{
  for(int bgCount = 0; bgCount < 1; bgCount++)
  {
        for(int numg = 0; numg < 4; numg++)
        {
          bGroup[bgCount].currentTime = millis();
          
          if(bGroup[bgCount].gest[numg].state == 0)                                                      //Control for State 0
          {
              if(buttonPressed[bGroup[bgCount].gest[numg].buttonOrder[0]] == true && (bGroup[bgCount].active == false))
              {
                bGroup[bgCount].gest[numg].state = 1;
                bGroup[bgCount].startTime = bGroup[bgCount].currentTime;
                leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[0]]].stops = 1;
                
                Serial.println("state1");
              }
              else
              {
                bGroup[bgCount].gest[numg].state = 0;  //Do Nothing
           //     leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[0]]].stops = 4;
                
              }
          }
          else if (bGroup[bgCount].gest[numg].state == 1)                                                 //Control for State 1
          {
              if(buttonPressed[bGroup[bgCount].gest[numg].buttonOrder[1]] == true)
              {
                bGroup[bgCount].gest[0].state = 0;
                bGroup[bgCount].gest[1].state = 0;
                bGroup[bgCount].gest[2].state = 0;
                bGroup[bgCount].gest[3].state = 0;
                
                bGroup[bgCount].gest[numg].state = 2;
                bGroup[bgCount].startTime = bGroup[bgCount].currentTime;
                leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[1]]].stops = 1;
                Serial.println("state2");
            //    if(numg == 2 || numg == 3)
                  bGroup[bgCount].active = true;
              }
              else if(buttonPressed[bGroup[bgCount].gest[numg].buttonOrder[0]] == true)
              {
                bGroup[bgCount].gest[numg].state = 1;
                leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[0]]].stops = 1;
                bGroup[bgCount].startTime = bGroup[bgCount].currentTime;
                Serial.println("state1");
              }
              else //Wait for Timer to expire
              {
                if(bGroup[bgCount].currentTime - bGroup[bgCount].startTime > bGroup[bgCount].wait)
                {
                  bGroup[bgCount].gest[numg].state = 0;
                  bGroup[bgCount].active = false;
                  leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[0]]].stops = 4;
                  
                  Serial.println("state0");
                }
                else 
                {
                  bGroup[bgCount].gest[numg].state = 1;
                  leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[0]]].stops = 1;
                  Serial.println("state1mm");
                }
              }
          }
          
          else if (bGroup[bgCount].gest[numg].state == 2)                                                 //Control for State 2
          {
              if(buttonPressed[bGroup[bgCount].gest[numg].buttonOrder[2]] == true)
              {
                bGroup[bgCount].gest[numg].state = 3;
                bGroup[bgCount].startTime = bGroup[bgCount].currentTime;
                leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[2]]].stops = 1;
                Serial.println("state3");
              }
              else if(buttonPressed[bGroup[bgCount].gest[numg].buttonOrder[1]] == true)
              {
                bGroup[bgCount].gest[numg].state = 2;
                bGroup[bgCount].startTime = bGroup[bgCount].currentTime;
                leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[1]]].stops = 1;
                Serial.println("state2");
              }
              else //Wait for Timer to expire
              {
                if(bGroup[bgCount].currentTime - bGroup[bgCount].startTime > bGroup[bgCount].wait)
                {
                  bGroup[bgCount].gest[numg].state = 1;
                  leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[1]]].stops = 4;
                  Serial.println("state1");
                }
                else 
                {
                  bGroup[bgCount].gest[numg].state = 2;
                  leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[1]]].stops = 1;
                  Serial.println("state2");
                }
              }
          }
          
          else if (bGroup[bgCount].gest[numg].state == 3)                                                 //Control for State 3
          {
              if(buttonPressed[bGroup[bgCount].gest[numg].buttonOrder[2]] == true)
              {
                bGroup[bgCount].gest[numg].state = 3;
                bGroup[bgCount].startTime = bGroup[bgCount].currentTime;
                leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[2]]].stops = 1;
                Serial.println("state3");
              }
              else
              {
                  bGroup[bgCount].gest[numg].state = 4;
                  
                  leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[0]]].stops = 2;
                  leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[1]]].stops = 2;
                  leds[ledxref[bGroup[bgCount].gest[numg].buttonOrder[2]]].stops = 2;
                  
                  Serial.println("state4a");
              }
          }
          else if(bGroup[bgCount].gest[numg].state == 4)
          {
            if(bGroup[bgCount].gest[numg].command == 0)
            {
              if(Fade_Backlight() == true)
                {
                  bGroup[bgCount].gest[numg].state = 5;
                  Serial.println("state5a");
                  fadeValue = -600;
                  Flip_Screen();
                  //delay(500);
                }
            }
            else if(bGroup[bgCount].gest[numg].command == 1)
            {
               if(Fade_Backlight() == true)
                {
                  bGroup[bgCount].gest[numg].state = 0;
                  bGroup[bgCount].active = false;
                  Lock_Screen();
                }
             } 
             else
             {
                bGroup[bgCount].gest[numg].state = 4;
               //    Serial.println("state4b");
             }
              
          }
          else if(bGroup[bgCount].gest[numg].state == 5)
          {
              if(Return_Backlight() == true)
                {
                  Serial.println("FADE Complete");
                  bGroup[bgCount].gest[numg].state = 0;
                  bGroup[bgCount].active = false;
                 }
               else
                {
                  //Serial.println("state5b");
                   bGroup[bgCount].gest[numg].state = 5;
                }
          }
          
          
        }
  }
}

bool gCommand(int cmd)
{}
  
   
