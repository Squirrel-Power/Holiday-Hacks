
//simple A4988 connection
//jumper reset and sleep together
//connect  VDD to Arduino 3.3v or 5v
//connect  GND to Arduino GND (GND near VDD)
//connect  1A and 1B to stepper coil 1
//connect 2A and 2B to stepper coil 2
//connect VMOT to power source (9v battery + term)
//connect GRD to power source (9v battery - term)

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

int stp = 13;  
int dir = 12; 

int maxPosition = 0;
int minPosition = 0;
boolean moveInProgress = false;
int currentPosition = 0;
int last_state_switch = 0;

int clockwise = 0;
int counterClockwise = 1;

const int pinButton = 4;

boolean buttonPressed = false;

const int debounce_time = 50;



void setup() 
{                
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);       
  
  lcd.begin(16, 2);
  
  pinMode(pinButton, INPUT);
  attachInterrupt(pinButton,switch_state, FALLING);
  last_state_switch = millis();

  autoCalibrate();
}


void loop() 
{
  while(!moveInProgress)
  {
    moveInProgress = true;
    rndMove();
  }
}

void autoCalibrate()
{
    
   while(!buttonPressed)
   {
     Move(clockwise, 1); 
   }
   maxPosition = currentPosition;
   
   buttonPressed = false;
  while(!buttonPressed)
  {
    Move(counterClockwise, 1);
  }
  minPosition = currentPosition;  
}

void Move (int direction, int steps)
{ 
  lcd.clear();
  
  if(direction == clockwise)
  {
    digitalWrite(dir, HIGH); 
  }
  else
  {
    digitalWrite(dir, LOW);
  }
    lcd.setCursor(0,1);
    lcd.print(direction);
  for(int x = 0; x < steps; x++)
  {
    lcd.setCursor(0,0);
    lcd.print(currentPosition);
    delay(100);
   if(direction == clockwise)
    {
      currentPosition++;
    }
    else
    {
      currentPosition--;
    } 
  }
  delay(100);
}

void rndMove()
{
  int rnd = random(minPosition, maxPosition + 1);
  int rndDirection = random(clockwise, counterClockwise + 1);
  
  if(rndDirection == clockwise)
  {
	if((rnd +  currentPosition) > maxPosition)
	{
            rnd = maxPosition - currentPosition-1;
	}
  }
  else
  {
    if((currentPosition - rnd) < minPosition)
    {
      rnd = currentPosition - minPosition+1;
    }
  }
  Move(rndDirection, rnd);
  moveInProgress = false;
}

void switch_state()
{  
    if((millis()- last_state_switch) > debounce_time)
    { 
        buttonPressed = true;
        last_state_switch = millis();
    }
  
}
