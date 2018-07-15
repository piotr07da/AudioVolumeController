/*
 * Usart.h
 *
 * Created: 7/10/2018 9:07:34 PM
 *  Author: piotr
 */ 


#ifndef USART_H_
#define USART_H_

class Usart
{
	public:
		void Init();
		void Send(uint8_t byte);
		void Send(uint8_t* bytes, uint16_t length);

};



#endif /* USART_H_ */