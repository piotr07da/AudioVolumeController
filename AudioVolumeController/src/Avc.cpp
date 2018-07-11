/*
 * Avc.cpp
 *
 * Created: 7/10/2018 10:09:53 PM
 *  Author: piotr
 */ 

#include "Avc.h"

#include <math.h>

Avc::Avc(DataReceiver* receiver, DataSender* sender, Accelerometer* accelerometer)
{
	_receiver = receiver;
	_sender = sender;
	_accelerometer = accelerometer;
	_receiver->SubscribeVolumeControlRequestReceiver(this);
}

void Avc::VolumeGetRequested()
{
	uint8_t x;
	uint8_t y;
	_accelerometer->ReadRegister(0x04, &x);
	_accelerometer->ReadRegister(0x06, &y);

	double angle = atan2(y, x);

	_sender->SendVolume((float)angle);
}

void Avc::VolumeSetRequested(float volume)
{


}