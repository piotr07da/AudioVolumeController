/*
 * Avc.h
 *
 * Created: 7/10/2018 9:05:54 PM
 *  Author: piotr
 */ 


#ifndef AVC_H_
#define AVC_H_


#include "DataReceiver.h"
#include "DataSender.h"
#include "Accelerometer.h"

class Avc : public IVolumeControllRequestsReceiver
{
	private:
		DataReceiver* _receiver;
		DataSender* _sender;
		Accelerometer* _accelerometer;

	public:
		Avc(DataReceiver* receiver, DataSender* sender, Accelerometer* accelerometer);

	public:
		virtual void VolumeGetRequested();
		virtual void VolumeSetRequested(float volume);

};


#endif /* AVC_H_ */