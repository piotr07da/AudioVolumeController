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

typedef enum AvcUpdateMode
{
	AvcUpdateMode_KeepTargetVolume,
	AvcUpdateMode_Calibrate
} AvcUpdateMode;

class Avc : public IVolumeControllRequestsReceiver
{
	private:
		DataReceiver* _receiver;
		DataSender* _sender;
		Accelerometer* _accelerometer;
		AvcUpdateMode _updateMode;
		float _targetVolume;
		float _minAngle;
		float _maxAngle;


		float _smoothRatio;
		float _prevVol;

	public:
		Avc(DataReceiver* receiver, DataSender* sender, Accelerometer* accelerometer);
		void Initialize();
		void Update();
		float CurrentVolume();

	private:
		void KeepTargetVolume();
		void Calibrate();
		float AngleToVolume(float angle);
		void TurnVolumeLeft();
		void TurnVolumeRight();
		void TurnVolumeStop();

	// IVolumeControllRequestsReceiver members.
	public:
		virtual void CalibrationRequested();
		virtual void VolumeGetRequested();
		virtual void VolumeSetRequested(float volume);
};


#endif /* AVC_H_ */