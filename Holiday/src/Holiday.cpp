/*
 * Author: Dustin Platter
 */

#include "mraa.hpp"

#include <iostream>
#include <unistd.h>

//#include "StepMotor.cpp"
#include "StepMotor.h"

int main()
{
	// select onboard LED pin based on the platform type
	// create a GPIO object from MRAA using it
	mraa_platform_t platform = mraa_get_platform_type();
	mraa::Gpio* d_pin = NULL;

	bool EdisonBoard = false;
	bool runProgram = true;

	StepMotor Ufo;

	//StepMotor* ptr_UfoMotor;
	//ptr_UfoMotor = new StepMotor();

	switch (platform) {
		case MRAA_INTEL_GALILEO_GEN1:
			d_pin = new mraa::Gpio(3, true, true);
			break;
		case MRAA_INTEL_GALILEO_GEN2:
			d_pin = new mraa::Gpio(13, true, false);
			break;
		case MRAA_INTEL_EDISON_FAB_C:
			d_pin = new mraa::Gpio(13, true, false);
			EdisonBoard = true;
			break;
		default:
			std::cerr << "Unsupported platform, exiting" << std::endl;
			return MRAA_ERROR_INVALID_PLATFORM;
	}

	//If this isn't an Edison board, exit the program
	if(EdisonBoard == false)
	{
		return MRAA_ERROR_INVALID_PLATFORM;
	}

	if (d_pin == NULL) {
		std::cerr << "Can't create mraa::Gpio object, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}

	// set the pin as output
	if (d_pin->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
		std::cerr << "Can't set digital pin as output, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}

	// loop forever toggling the on board LED every second
	while (runProgram) {
		d_pin->write(1);
		sleep(1);
		d_pin->write(0);
		sleep(1);

		//(*UfoMotor).RndMove();
		//*UfoMotor->RndMove();
		//UfoMotor->RndMove();
		//UfoMotor.RndMove();
		Ufo.MyRndMove();

		sleep(10);
	}

	return MRAA_SUCCESS;
}
