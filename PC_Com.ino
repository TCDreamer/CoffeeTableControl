

void Flip_Screen()
{
  
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_UP_ARROW);
    Keyboard.releaseAll();

};

void Start_Pressed(char button)
{
  if(button = 'F')
    Serial.println("Front Start Pressed");
  else if (button = 'R')
    Serial.println("Rear Start Pressed");
}

void Start_Released(char button)
{
  if(button = 'F')
    Serial.println("Front Start Released");
  else if(button = 'R')
    Serial.println("Rear Start Released");
    
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_ESC);
  //Keyboard.press(KEY_LEFT_GUI);
  Keyboard.releaseAll();  
  
}

void Lock_Screen()
{
  
  Keyboard.press(KEY_LEFT_GUI);
  
  Keyboard.press('l'); 
  Keyboard.releaseAll();  
}
  

