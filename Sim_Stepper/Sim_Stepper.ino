/*
  Blink.ino
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-9-18

  Grove - Serial LCD RGB Backlight demo.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Wire.h>
#include "rgb_lcd.h"
#include <Servo.h>

rgb_lcd lcd;
Servo groveServo;

const int stp = 13;  //connect pin 13 to step
const int dir = 12;  // connect pin 12 to dir
const int pinButton = 3;
int steps = 0;
int direc = 1;
int last_state_switch = 0;
const int debounce_time = 50;
const int pinServo = 2;
int servo_step = 0;
void setup() 
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    pinMode(stp, OUTPUT);
    pinMode(dir, OUTPUT); 
    digitalWrite(dir,LOW);
    pinMode(pinButton, INPUT);
    groveServo.attach(pinServo);
    attachInterrupt(pinButton,switch_state, FALLING);
    // Print a message to the LCD.
    
    last_state_switch = millis();
    delay(1000);
}

void loop() 
{
  if(servo_step > 180)
     servo_step = 0;
  else if(servo_step <0);
      servo_step= 0;
  servo_step += direc;
  steps += direc;
  lcd.print(steps);
  groveServo.write(servo_step);
  delay(100);
  lcd.clear();
}

void switch_state()
{  
    if((millis()- last_state_switch) > debounce_time)
    { 
        steps = 0;
        direc *= -1;
        last_state_switch = millis();
    }
  
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
