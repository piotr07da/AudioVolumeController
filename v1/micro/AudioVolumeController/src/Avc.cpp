/*
 * Avc.cpp
 *
 * Created: 7/10/2018 10:09:53 PM
 *  Author: piotr
 */ 

#include "Avc.h"

#include <math.h>
#include <avr/io.h>

Avc::Avc(DataReceiver* receiver, DataSender* sender, Accelerometer* accelerometer)
{
	_receiver = receiver;
	_sender = sender;
	_accelerometer = accelerometer;

	_minAngle = -110;
	_maxAngle = 165;

	_prevVol = 0;
}

void Avc::Initialize()
{
	_receiver->SubscribeVolumeControlRequestReceiver(this);
	_smoothRatio = 1;
	_targetVolume = CurrentVolume();
	_smoothRatio = .02;
	_updateMode = AvcUpdateMode_KeepTargetVolume;
}

void Avc::Update()
{
	switch (_updateMode)
	{
		case AvcUpdateMode_Calibrate:
			Calibrate();
			break;

		case AvcUpdateMode_KeepTargetVolume:
			KeepTargetVolume();
			break;
	}
}

float Avc::CurrentVolume()
{

	float x0;
	float x1;
	_accelerometer->ReadAxisAcceleration(1, &x0);
	_accelerometer->ReadAxisAcceleration(2, &x1);
	
	float mag = (float)sqrt(x0 * x0 + x1 * x1);
	x0 /= mag;
	x1 /= mag;

	float angle = (float)atan2(x1, x0);
	angle = 180.0 * angle / M_PI;

	float vol = (angle - _minAngle) / (_maxAngle - _minAngle);

	_prevVol = _prevVol + _smoothRatio * (vol - _prevVol);
	return _prevVol;
}

void Avc::KeepTargetVolume()
{
	float vol = CurrentVolume();
	float diff = _targetVolume - vol;
	if (diff > .002)
	{
		TurnVolumeRight();
	}
	else if (diff < -.002)
	{
		TurnVolumeLeft();
	}
	else
	{
		TurnVolumeStop();
	}
}

void Avc::Calibrate()
{
	
}

float Avc::AngleToVolume(float angle)
{
	return .5;
}

void Avc::TurnVolumeLeft()
{
	PORTD |= _BV(6);
	PORTD &= ~_BV(7);
}

void Avc::TurnVolumeRight()
{
	PORTD &= ~_BV(6);
	PORTD |= _BV(7);
}

void Avc::TurnVolumeStop()
{
	PORTD &= ~_BV(6);
	PORTD &= ~_BV(7);
}

void Avc::CalibrationRequested()
{
	_updateMode = AvcUpdateMode_Calibrate;
}

void Avc::VolumeGetRequested()
{
	float vol = CurrentVolume();
	_sender->SendVolume(vol);
}

void Avc::VolumeSetRequested(float volume)
{
	_targetVolume = volume;
}