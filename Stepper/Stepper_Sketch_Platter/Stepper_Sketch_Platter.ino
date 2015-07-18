
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

int stp = 13;  //connect pin 13 to step
int dir = 12;  // connect pin 12 to dir
int a = 0;     //  gen counter

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
    lcd.print("S
    moveInProgress = true;
    rndMove();
  }
  

}

void autoCalibrate()
{
    
   //Get MaxValue
   while(!buttonPressed)
   {
     Move(clockwise, 1);  //Move clockwise until button press is detected
   }
   maxPosition = currentPosition;
   
   buttonPressed = false;
  while(!buttonPressed)
  {
    Move(counterClockwise, 1);  //Move one step counter-clockwise until button press is detected;
  }
  minPosition = currentPosition;  
}

void Move (int direction, int steps)
{
  //Set direction
  if(direction == clockwise)
  {
    digitalWrite(dir, HIGH); 
  }
  else
  {
    digitalWrite(dir, LOW);
  }

  //Move indicated number of steps
  for(int x = 0; x < steps; x++)
  {
    /*digitalWrite(stp, HIGH);   
    delay(10);               
    digitalWrite(stp, LOW);  
    delay(10);*/
    currentPosition;
    lcd.clear();
    lcd.print(currentPosition);
    delay(100);
    /*if(direction == clockwise)
    {
      currentPosition++;
    }
    else
    {
      currentPosition--;
    }  */
  }
}

void rndMove()
{
  //int rnd = rand() % 100 + 1;	  //Determine random distance to move
  int rnd = random(minPosition, maxPosition);
  //int rndDirection = rand() % 100;  //Pick random direction
  int rndDirection = random(clockwise, counterClockwise);
  
  //if(rndDirection % 2 == 0)	//Move towards Max value
  if(rndDirection == clockwise)
  {
	if(rnd +  currentPosition > maxPosition)
	{
            rnd = maxPosition - currentPosition;
	}
  }
  else	//Move towards Min value
  {
    if(currentPosition - rnd < minPosition)
    {
      rnd = currentPosition - minPosition;
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
