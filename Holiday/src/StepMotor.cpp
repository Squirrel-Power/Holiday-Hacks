/*
 * StepMotor.cpp
 *
 *  Created on: Jun 14, 2015
 *      Author: Cyber
 */

#include "StepMotor.h"
#include <stdlib.h>

StepMotor::StepMotor() {
	MaxDist = GetMax();	//Determine Max distance for motor
	MinDist = GetMin();	//Determine Min distance for motor
	StepMotor::CurrentPosition = StepMotor::MinDist;	//Set default position at Min distance
	StepMotor::myServo = new SoftwareServo(5);

StepMotor::~StepMotor() {
	// TODO Auto-generated destructor stub
}

/*
 * GetMax Function
 * This function will trigger the motor to run until hitting its max length.
 */
int GetMax(){
	int max_distance = 100;

	return max_distance;
}

/*
 * GetMin Function
 * This function will trigger the motor to run until hitting its min length.
 */
int GetMin(){
	int min_distance = 0;

	return min_distance;
}

/*
 * Random Move Function
 * This function will trigger the motor to move a random distance between the
 * Minimum and Maximum distance values.
 */
void RndMove()
{
	int rnd = rand() % 100 + 1;				//Determine random distance to move
	int rndDirection = rand() % 100;	//Pick random direction

	if(rndDirection % 2 == 0)	//Move towards Max value
	{
		if(rnd +  StepMotor::CurrentPosition > StepMotor::MaxDist)
		{
			rnd = StepMotor::MaxDist - StepMotor::CurrentPosition;
		}
	}
	else	//Move towards Min value
	{
		if(StepMotor::CurrentPosition - rnd < StepMotor::MinDist)
		{
			rnd = StepMotor::CurrentPosition - StepMotor::MinDist;
		}
	}
	Move(rnd);
}

/*
 * Move Function
 * This function will trigger the motor to move an object a specified amount.
 * Parameters (Distance)
 */
void Move(int Distance)
{
	//NOTE: This code needs determined. This will be based mostly on the motor utilized.
	std::cout << StepMotor::CurrentPosition;
	StepMotor::myServo.write(Distance);
}
