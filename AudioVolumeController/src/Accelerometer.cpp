/*
 * Accelerometer.cpp
 *
 * Created: 7/10/2018 2:27:28 PM
 *  Author: piotr
 */ 

#include "Accelerometer.h"
#include "Wire.h"

void Accelerometer::Setup()
{
	Wire.beginTransmission(0x0A);
	Wire.write(0x22); // set sensitivity register
	Wire.write(0x00); // write mode value to sensitivity register
	Wire.write(0x20); // set filter register
	Wire.write(0x00); // write mode value to filter register
	Wire.endTransmission();
}

bool Accelerometer::ReadRegister(uint8_t registerAddress, uint8_t* byte)
{
	Wire.beginTransmission(0x0A);
	Wire.write(registerAddress);
	Wire.endTransmission();
	Wire.requestFrom(0x0A, 1);
	if (Wire.available())
	{
		*byte = Wire.read();
		return true;
	}
	return false;
}