# CoffeeTableControl
Arduino controller for peripherals of my touchscreen coffee table

The coffee table is a Windows 10 PC connected to a disassembled LED TV stuffed into a coffee table. 
Using 4 PS3 Eye cameras and an array of IR LEDS and CCV 1.5 from NUIGroup, touches and gestures on the screen can be captured.

Around the screen, there are several capacitive sensors, ambient Light/IR sensors, and LEDS that control different actions of the table.
The Arduino code here is responsible for capturing the data from the sensors and illuminate the LEDs and performing actions on the PC (like flipping the screen)
It also controls the power MOSFETS that control the LED backlighting.

The code was built for and Arduino Leonardo and will not run on the Uno without removing the keyboard press methods.

THe LEDs are controlled through 2 daisy chained 74HC595 shift registers. The code using the ShiftPWM library to enable PWM dimming of the LEDs through the register.
The Atmel QT series capacitive sensors return their state through daisy chained 74HC165N shift registers.

Analog input from the light sensors is not implemented here, but will be passed into the Arduino by a multiplexer on a separate board.
