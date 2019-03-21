#ifndef LED_h
#define LED_h


class led
{
public:
//typedef struct led{
  int currentLevel;
  bool active;
  bool half;
  int stops;
//};  
};
/*
class gesture
{
  public:
  */
  
class gestureBlock
{
  public:
  
  typedef struct gesture
  {
    int buttons[3];
    int command;
  } gesture;
  
  int block;
  
  int lastPressed;
  int currentPressed;
 
  unsigned long lastTime;       //The last time returned from Millis that a button was pressed
  unsigned long currentTime;    //The current time returned by Millis
  
  unsigned int wait;            //How many Millis to wait before gesture is cancelled if no buttons are pressed in the button block
  
  int state;                    /*The current state of the gesture block. 0 = inactive(Any Gesture can start) 
                                                                          1 = in process(Only current Gesture can continue) 
                                                                          2 = Gesture Confirmed(Execute Action, LED confirmation)
                                                                          3 = Gesture Completion(All features return to normal. LEDs fade to 0, Buttons reset, Action completes)*/
                                                                         
  int activeGesture;
  
  gesture gestures[4];
  
};
  

#endif
