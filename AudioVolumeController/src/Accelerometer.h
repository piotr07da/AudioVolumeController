/*
 * Accelerometer.h
 *
 * Created: 7/10/2018 2:27:41 PM
 *  Author: piotr
 */ 


#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <avr/io.h>

class Accelerometer
{
	public:
		void Setup();
		bool ReadRegister(uint8_t registerAddress, uint8_t* byte);

};


#endif /* ACCELEROMETER_H_ */