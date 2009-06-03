#ifndef AVRLIB_USART1_HPP
#define AVRLIB_USART1_HPP

#include <avr/io.h>
#include <stdint.h>

namespace avrlib {

class usart1
{
public:
	typedef uint8_t value_type;

	explicit usart1(uint16_t ubrr)
	{
		UBRR1H = ubrr >> 8;
		UBRR0H = ubrr & 0xFF;
		UCSR1A = (1<<U2X1);
		UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
		UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	}

	~usart1()
	{
		UCSR1B = 0;
	}

	void send(value_type v)
	{
		UDR1 = v;
	}

	value_type recv()
	{
		return UDR1;
	}

	bool rx_empty() const
	{
		return UCSR1A & (1<<RXC1);
	}

	bool tx_empty() const
	{
		return UCSR1A & (1<<UDRE1);
	}
};

}

#endif
