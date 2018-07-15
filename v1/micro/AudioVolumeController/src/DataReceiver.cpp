/*
 * Communicator.cpp
 *
 * Created: 7/11/2018 11:03:26 AM
 *  Author: piotr
 */ 

#include "DataReceiver.h"

DataReceiver::DataReceiver()
{
	_byteIx = -1;
	_frameReceived = false;
}

void DataReceiver::SubscribeVolumeControlRequestReceiver(IVolumeControllRequestsReceiver* volumeControllRequestsReceiver)
{
	_volumeControllRequestsReceiver = volumeControllRequestsReceiver;
}

bool DataReceiver::ReceiveByte(uint8_t byte)
{
	_frameReceived = false;
	++_byteIx;

	if (_byteIx == 0 || _byteIx == 1)
	{
		// First two header bytes.
		if (byte == 0xAA)
			return true;
		ResetByteIndex();
		return false;
	}

	if (_byteIx == 2)
	{
		// Third header byte.
		if (byte == 0xFF)
			return true;
		ResetByteIndex();
		return false;
	}
	
	if (_byteIx == 3)
	{
		// Fourth header byte.
		if (byte == 0x00)
			return true;
		ResetByteIndex();
		return false;
	}
	
	if (_byteIx == 4)
	{
		// Message type byte.
		_messageType = byte;
		return true;
	}
	
	if (_byteIx == 5)
	{
		// Message content length byte.
		_messageLength = byte;
		return true;
	}

	if (_byteIx >= 6 && _byteIx < 6 + _messageLength)
	{
		// Message content bytes.
		_messageBuffer[_byteIx - 6] = byte;
		return true;
	}

	if (_byteIx == 6 + _messageLength)
	{
		// Control byte.
		ResetByteIndex();
		bool messageCorrect = CalculateControlByte() == byte;
		if (messageCorrect)
		{
			
			_frameReceived = true;
		}
		return messageCorrect;
	}

	// This case will never happen.
	ResetByteIndex();
	return false;
}

bool DataReceiver::FrameReceived(uint8_t* controlByte)
{
	*controlByte = _controlByte;
	return _frameReceived;
}

void DataReceiver::AnalyzeReceivedFrame()
{
	if (!_frameReceived)
		return;

	switch (_messageType)
	{
		case 0x50:
			_volumeControllRequestsReceiver->VolumeGetRequested();
			break;

		case 0x51:
			_volumeControllRequestsReceiver->VolumeSetRequested(*(float*)_messageBuffer);
			break;
	}
}

void DataReceiver::ResetByteIndex()
{
	_byteIx = -1;
}

uint8_t DataReceiver::CalculateControlByte()
{
	uint8_t controlByte = 0x00;
	controlByte ^= _messageType;
	controlByte ^= _messageLength;
	for (int32_t i = 0; i < _messageLength; ++i)
	{
		controlByte ^= _messageBuffer[i];
	}
	return controlByte;
}