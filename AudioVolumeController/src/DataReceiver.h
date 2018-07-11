/*
 * Communicator.h
 *
 * Created: 7/10/2018 9:07:04 PM
 *  Author: piotr
 */ 
 
// FRAME:
// | 2 bytes - 0xAA | 1 byte - 0xFF | 1 byte - 0x00 | 1 byte - message type | 1 byte - message length | x bytes - message | 1 byte - control byte |

#ifndef DATARECEIVER_H_
#define DATARECEIVER_H_

#include <avr/io.h>

#define Communicator_MaxMessageLength 16

class IVolumeControllRequestsReceiver
{
	public:
		virtual void VolumeGetRequested() = 0;
		virtual void VolumeSetRequested(float volume) = 0;
};

class DataReceiver
{
	private:
		uint8_t _byteIx;
		uint8_t _messageType;
		uint8_t _messageLength;
		uint8_t _messageBuffer[Communicator_MaxMessageLength];
		uint8_t _controlByte;
		bool _frameReceived;
		IVolumeControllRequestsReceiver* _volumeControllRequestsReceiver;

	public:
		DataReceiver();
		void SubscribeVolumeControlRequestReceiver(IVolumeControllRequestsReceiver* volumeControllRequestsReceiver);
		bool ReceiveByte(uint8_t byte);
		bool FrameReceived(uint8_t* controlByte);

	private:
		void AnalyzeReceivedFrame();
		void ResetByteIndex();
		uint8_t CalculateControlByte();
};



#endif /* DATARECEIVER_H_ */