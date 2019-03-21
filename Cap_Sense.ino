 /* SN74HC165N_shift_reg
  *
  * Program to shift in the bit values from a SN74HC165N 8-bit
  * parallel-in/serial-out shift register.
  *
  * This sketch demonstrates reading in 16 digital states from a
  * pair of daisy-chained SN74HC165N shift registers while using
  * only 4 digital pins on the Arduino.
  *
  * You can daisy-chain these chips by connecting the serial-out
  * (Q7 pin) on one shift register to the serial-in (Ds pin) of
  * the other.
  *
  * Of course you can daisy chain as many as you like while still
  * using only 4 Arduino pins (though you would have to process
  * them 4 at a time into separate unsigned long variables).
  *
 */

 #include "led.h"

#define NUMBER_OF_SHIFT_CHIPS   2                   //Number of Registers that are daisy chained together

#define DATA_WIDTH   NUMBER_OF_SHIFT_CHIPS * 8      //Number of inputs. Each register has 8 inputs individually

#define PULSE_WIDTH_USEC   5                        //Number of microseconds that are waited between reading each input and reseting the latch pin.

#define POLL_DELAY_MSEC   1                         //How long is waited between each read, optional

#define BYTES_VAL_T unsigned int                    //You will need to change the "int" to "long". If the NUMBER_OF_SHIFT_CHIPS is higher than 2.

int ploadPin = 7;                                   // Connects to Parallel load pin the 165
int dataPin  = 4;                                   // Connects to the Q7 pin the 165
int clockPin = 5;                                   // Connects to the Clock pin the 165

//unsigned int pinValues;
//unsigned int oldPinValues;
 
bool buttonPressed[16];
 

 /* This function is essentially a "shift-in" routine reading the serial Data from the shift register chips and representing
    the state of those pins in an unsigned integer (or long).*/
 unsigned int read_shift_regs()
 {
     byte bitVal;
     unsigned int bytesVal = 0;

     // Trigger a parallel Load to latch the state of the data lines
     digitalWrite(ploadPin, LOW);
     delayMicroseconds(PULSE_WIDTH_USEC);
     digitalWrite(ploadPin, HIGH);

     // Loop to read each bit value from the serial out line of the SN74HC165N.
     for(int i = 0; i < DATA_WIDTH; i++)
     {
       
         bitVal = digitalRead(dataPin);

         // Set the corresponding bit in bytesVal.
         bytesVal |= (bitVal << ((DATA_WIDTH-1) - i));

         // Pulse the Clock (rising edge shifts the next bit).
         digitalWrite(clockPin, HIGH);
         delayMicroseconds(PULSE_WIDTH_USEC);
         digitalWrite(clockPin, LOW);
     }
     
     return(bytesVal);

 }

 //Dump the list of zones along with their current status.
 
 void display_pin_values()
 {
     for(int i = 0; i < DATA_WIDTH; i++)
     {
       
         if((pinValues >> i) & 1)
         {
             buttonPressed[i] = true;
             Serial.print("1");
         }
         else
         {
             buttonPressed[i] = false;
             Serial.print("0");
         }

         Serial.print(",");
     }

     Serial.println();
 }
 
 void CapSense_Setup(void){
     pinMode(ploadPin, OUTPUT);
     pinMode(clockPin, OUTPUT);
     pinMode(dataPin, INPUT);

     digitalWrite(clockPin, LOW);
     digitalWrite(ploadPin, HIGH);
     unsigned long currentTime = millis();
     //Read in and display the pin states at startup.
     //Do While added to hopefully stall the app until the 74hc165 initializes. 
     //It waits until the pins returned are returned to their off state.
     do {
       pinValues = read_shift_regs();
     } while(millis()-currentTime<1000);//while(pinValues != 16||counting<1000);
     
     display_pin_values();
     oldPinValues = pinValues;
 }
 
 
 
 void bGroupInit()
{
  bGroup[0].wait = 500;
  bGroup[0].currentTime = 0;
  bGroup[0].startTime = 0;
  bGroup[0].active = false;
  
  bGroup[0].gest[0].gID = 11;
  
    bGroup[0].gest[0].buttonOrder[0] = 14;  
    bGroup[0].gest[0].buttonOrder[1] = 13;
    bGroup[0].gest[0].buttonOrder[2] = 12;
    bGroup[0].gest[0].command = 0;
    
   bGroup[0].gest[0].state = 0;
 
   bGroup[0].gest[1].gID = 12;
    bGroup[0].gest[1].buttonOrder[0] = 10;  
    bGroup[0].gest[1].buttonOrder[1] = 11;
    bGroup[0].gest[1].buttonOrder[2] = 12;
    bGroup[0].gest[1].command = 0;
    
    bGroup[0].gest[1].state = 0;
    
   bGroup[0].gest[2].gID = 13;
    bGroup[0].gest[2].buttonOrder[0] = 12;  
    bGroup[0].gest[2].buttonOrder[1] = 11;
    bGroup[0].gest[2].buttonOrder[2] = 10;
    bGroup[0].gest[2].command = 1;
    
    bGroup[0].gest[2].state = 0;
    
   bGroup[0].gest[3].gID = 14;
    bGroup[0].gest[3].buttonOrder[0] = 12;  
    bGroup[0].gest[3].buttonOrder[1] = 13;
    bGroup[0].gest[3].buttonOrder[2] = 14;
    bGroup[0].gest[3].command = 1;
    
    bGroup[0].gest[3].state = 0;
}
