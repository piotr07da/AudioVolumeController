/*
 * AudioVolumeController.c
 *
 * Created: 6/11/2018 12:05:14 AM
 * Author : piotr
 */ 

#define F_CPU 14745600UL

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Avc.h"
#include "DataReceiver.h"
#include "DataSender.h"
#include "Accelerometer.h"
#include "Wire.h"

#define __avc_BR 9600UL // bit rate
#define __avc_UBRR (F_CPU / (16UL * __avc_BR)) - 1 // usart baud rate register

void InitUSART(uint8_t ubrr);
void SendByteUSART(uint8_t byte);
void InitTinyWireM();


void SendValue(int8_t v);
void SendValue(float v);

Avc* _avc;
DataReceiver _receiver;
DataSender _sender;
Accelerometer _acc;

float _x, _y, _z;

volatile uint8_t _b = 0x00;
volatile bool _bReady = false;

int main(void)
{
	Avc avc = Avc(&_receiver, &_sender, &_acc);
	_avc = &avc;

	InitUSART(__avc_UBRR);
	
	Wire.begin();

	DDRB |= _BV(0);
	DDRD |= _BV(5) | _BV(6) | _BV(7);

	PORTB |= _BV(0);
	PORTD |= _BV(5) | _BV(6) | _BV(7);

	sei();

	SendByteUSART('f');

	_avc->Initialize();

	_delay_ms(1000);

    while (1) 
    {
		_delay_ms(10);

		_avc->Update();
	
		if (false)
		{
			float vol = _avc->CurrentVolume();
			SendByteUSART(' ');
			SendValue(vol);
			SendByteUSART(' ');
		}
		
		//if (_acc.ReadAxisAcceleration(0, &_x) && _acc.ReadAxisAcceleration(1, &_y) && _acc.ReadAxisAcceleration(2, &_z))
		if (false)
		{
			SendByteUSART('#');
			SendValue(_x);
			SendByteUSART(' ');
			SendValue(_y);
			SendByteUSART(' ');
			SendValue(_z);
			SendByteUSART('#');
		}
	
		//_delay_ms(5000);

		if (_bReady)
		{
			float vol = _avc->CurrentVolume();
			SendByteUSART(' ');
			SendValue(vol);
			SendByteUSART('^');


		

			//_receiver.ReceiveByte(b);
			//
			//uint8_t controlByte;
			//if (_receiver.FrameReceived(&controlByte))
			//{
			//SendByteUSART(0xAA);
			//SendByteUSART(controlByte);
			//SendByteUSART(0xAA);
			//}

			_bReady = false;
		}
    }
}

ISR (USART_RXC_vect)
{
	_b = UDR;
	_bReady = true;
}

void InitUSART(uint8_t ubrr)
{
	// Set the baud rate.
	UBRRH = (unsigned char)(ubrr >> 8);
	UBRRL = (unsigned char)(ubrr);
	
	// Enable UART receiver and transmitter and receiver interrupt handlings
	UCSRB = _BV(RXEN) | _BV(TXEN) | _BV(RXCIE);
	
	// Set to 8 data bits, 1 stop bit, async
	UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);
}

void SendByteUSART(uint8_t byte)
{
	while (!(UCSRA & _BV(UDRE))) ;
	UDR = byte;
}

void SendValue(int8_t v)
{
	char c[4];
	for (int i = 0; i < 4; ++i)
	c[i] = '*';
	itoa((int)v, c, 10);
	for (int i = 0; i < 4; ++i)
	SendByteUSART(c[i]);
}

void SendValue(float v)
{
	char s[10];
	for (int i = 0; i < 10; ++i)
		s[i] = '#';

	dtostrf(v, 10, 5, s);
	
	for (int i = 0; i < 10; ++i)
		SendByteUSART(s[i]);
}