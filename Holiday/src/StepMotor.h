/*
 * StepMotor.h
 *
 *  Created on: Jun 14, 2015
 *      Author: Cyber
 */

#ifndef STEPMOTOR_H_
#define STEPMOTOR_H_

//#include "SoftwareServo.h"
#include <stdlib.h>

class StepMotor {
public:
	StepMotor();
	virtual ~StepMotor();
	void MyRndMove();
	int MaxDist;
	int MinDist;
	int CurrentPosition;

private:
	int GetMax();
	int GetMin();

	void Move(int distance);
	//SoftwareServo myServo;
};

#endif /* STEPMOTOR_H_ */
