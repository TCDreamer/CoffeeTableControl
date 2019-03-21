#ifndef LED_h
#define LED_h


class s_led
{
public:
  int currentLevel;
  bool active;
  bool half;
  int stops;
    
};

  

class buttonGroup
{
  public:
    
    typedef struct Gesture
    {
      int gID;
      int buttonOrder[3];
      int state;
      int command;
    } Gesture;
    
    Gesture gest[4];
    
    unsigned long startTime;
    unsigned long currentTime;
    
    unsigned int wait;
    
    bool active;
    
};



#endif
