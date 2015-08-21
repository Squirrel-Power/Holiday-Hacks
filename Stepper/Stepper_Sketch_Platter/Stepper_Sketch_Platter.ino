
//simple A4988 connection
//jumper reset and sleep together
//connect  VDD to Arduino 3.3v or 5v
//connect  GND to Arduino GND (GND near VDD)
//connect  1A and 1B to stepper coil 1
//connect 2A and 2B to stepper coil 2
//connect VMOT to power source (9v battery + term)
//connect GRD to power source (9v battery - term)

#include <Wire.h>
//#include "rgb_lcd.h"

//rgb_lcd lcd;

int stp = 13;  
int dir = 12; 
int rst = 8;

int maxPosition = 0;
int minPosition = 0;
boolean moveInProgress = false;
int currentPosition = 0;
int last_state_switch = 0;
int last_state_switch2 = 0;
int count_pinbutton2 = 0;

int clockwise = 0;
int counterClockwise = 1;

const int pinButton = 4;
const int pinButton2 = 3;

boolean buttonPressed = false;
boolean buttonPressed2 = false;

const int debounce_time = 50;



void setup() 
{                
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);       
  digitalWrite(rst,LOW);
  delay(10);
  digitalWrite(rst,HIGH);
  Serial1.begin(9600);
  pinMode(pinButton, INPUT);
  pinMode(pinButton2, INPUT);
  attachInterrupt(pinButton,switch_state, FALLING);
  attachInterrupt(pinButton2,switch_state2, FALLING);
  last_state_switch = millis();
  last_state_switch2 = millis();

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

void one_step()
{
    digitalWrite(stp, HIGH);   
    delay(10);               
    digitalWrite(stp, LOW);  
    delay(10); 
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
  
  if(direction == clockwise)
  {
    digitalWrite(dir, HIGH); 
  }
  else
  {
    digitalWrite(dir, LOW);
  }
  
  for(int x = 0; x < steps; x++)
  {
   // delay(100);
    one_step();
   if(direction == clockwise)
    {
      currentPosition++;
    }
    else
    {
      currentPosition--;
    } 
  }
 // delay(100);
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

void switch_state2()
{  
    if((millis()- last_state_switch2) > debounce_time)
    {   
       if(count_pinbutton2%3 == 0)
          Serial1.print("c");
        else if(count_pinbutton2%3 == 1)
          Serial1.print("h");
        else if(count_pinbutton2%3 == 2)
          Serial1.print("x");
        last_state_switch2 = millis();
        count_pinbutton2++;
    }
  
}
