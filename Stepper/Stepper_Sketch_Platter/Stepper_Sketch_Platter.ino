
//simple A4988 connection
//jumper reset and sleep together
//connect  VDD to Arduino 3.3v or 5v
//connect  GND to Arduino GND (GND near VDD)
//connect  1A and 1B to stepper coil 1
//connect 2A and 2B to stepper coil 2
//connect VMOT to power source (9v battery + term)
//connect GRD to power source (9v battery - term)


int stp = 13;  //connect pin 13 to step
int dir = 12;  // connect pin 12 to dir
int a = 0;     //  gen counter

int maxPosition = 0;
int minPosition = 0;
boolean moveInProgress = false;
int currentPosition = 0;

void setup() 
{                
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);       
  
  autoCalibrate();
}


void loop() 
{
  while(!moveInProgress)
  {
    MoveInProgress = true;
    rndMove();
  }
  
  /*if (a <  200)  //sweep 200 step in dir 1
   {
    a++;
    digitalWrite(stp, HIGH);   
    delay(10);               
    digitalWrite(stp, LOW);  
    delay(10);              
   }
  else 
   {
    digitalWrite(dir, HIGH);
    a++;
    digitalWrite(stp, HIGH);  
    delay(10);               
    digitalWrite(stp, LOW);  
    delay(10);
    
    if (a>400)    //sweep 200 in dir 2
     {
      a = 0;
      digitalWrite(dir, LOW);
     }
    }*/
}

void autoCalibrate()
{
   boolean buttonPressed = false;
   
   //Get MaxValue
   while(!buttonpressed)
   {
     Move(1, 1);  //Move clockwise until button press is detected
     currentPosition++;
   }
   maxPosition = stepPosition;
   
   buttonPressed = false;
  while(!buttonPressed)
  {
    Move(0, 1);  //Move one step counter-clockwise until button press is detected;
    currentPosition--;
  }
  minPosition = stepPosition;  
}

void Move (int direction, int steps)
{
  //Set direction
  if(direction == 1)
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
    digitalWrite(stp, HIGH);   
    delay(10);               
    digitalWrite(stp, LOW);  
    delay(10);
  }
}

void rndMove()
{
  int rnd = rand() % 100 + 1;	  //Determine random distance to move
  int rndDirection = rand() % 100;  //Pick random direction
  
  if(rndDirection % 2 == 0)	//Move towards Max value
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
