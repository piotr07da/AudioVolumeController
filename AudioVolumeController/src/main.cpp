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

#include "ExternalCode/TinyWireM/TinyWireM.h"

#include "Avc.h"
#include "DataReceiver.h"
#include "DataSender.h"
#include "Accelerometer.h"

#define __avc_BR 9600UL // bit rate
#define __avc_UBRR (F_CPU / (16UL * __avc_BR)) - 1 // usart baud rate register

void InitUSART(uint8_t ubrr);
void SendByteUSART(uint8_t byte);
void InitTinyWireM();


void SendValue(int8_t v);

volatile uint8_t x, y, z;

Avc* _avc;
DataReceiver _receiver;
DataSender _sender;
Accelerometer _acc;

int main(void)
{
	Avc avc = Avc(&_receiver, &_sender, &_acc);
	//Avc* avc = new Avc(NULL, NULL, NULL);

	InitUSART(__avc_UBRR);
	TinyWireM.begin();

	DDRB |= _BV(0);
	DDRD |= _BV(6);

	sei();

    while (1) 
    {
	/*
		PORTB &= ~(1 << 0);
		PORTD |= 1 << 6;
		_delay_ms(500);
		PORTB |= 1 << 0;
		PORTD &= ~(1 << 6);
		_delay_ms(500);
	*/

		_delay_ms(3000);
		
		int8_t b;
		if (_acc.ReadRegister(0x04, (uint8_t*)&b))
		{
			b >>= 2;
			//SendValue(b);
		}
    }

	//SendByteUSART((uint8_t)74);
}

ISR (USART_RX_vect)
{
	uint8_t b = UDR;

	_receiver.ReceiveByte(b);

	uint8_t controlByte;
	if (_receiver.FrameReceived(&controlByte))
	{
		SendByteUSART(0xAA);
		SendByteUSART(controlByte);
		SendByteUSART(0xAA);
	}
}

void InitUSART(uint8_t ubrr)
{
	// Set the baud rate.
	UBRRH = (unsigned char)(ubrr >> 8);
	UBRRL = (unsigned char)(ubrr);
	
	// Enable UART receiver and transmitter and receiver interrupt handlings
	UCSRB = _BV(RXEN) | _BV(TXEN) | _BV(RXCIE);
	
	// Set to 8 data bits, 1 stop bit, async
	UCSRC = _BV(UCSZ1) | _BV(UCSZ0);
}

void SendByteUSART(uint8_t byte)
{
	while (!(UCSRA & _BV(UDRE))) ;
	UDR = byte;
	//PORTB ^= _BV(0);
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