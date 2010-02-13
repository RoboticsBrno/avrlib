#ifndef AVRLIB_USART0_HPP
#define AVRLIB_USART0_HPP

#include <avr/io.h>
#include <stdint.h>

namespace avrlib {

class usart0
{
public:
	typedef uint8_t value_type;

	usart0(uint16_t ubrr, bool rx_interrupt)
	{
		UBRR0H = ubrr >> 8;
		UBRR0L = ubrr & 0xFF;
		UCSR0A = (1<<U2X0);
		UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
		UCSR0B = (1<<RXEN0)|(1<<TXEN0);

		if (rx_interrupt)
			UCSR0B |= (1<<RXCIE0);
	}

	~usart0()
	{
		UCSR0B = 0;
	}

	void send(value_type v)
	{
		UDR0 = v;
	}

	bool overflow() const
	{
		return (UCSR0A & (1<<DOR0)) != 0;
	}

	value_type recv()
	{
		return UDR0;
	}

	bool rx_empty() const
	{
		return (UCSR0A & (1<<RXC0)) == 0;
	}

	bool tx_empty() const
	{
		return (UCSR0A & (1<<UDRE0)) != 0;
	}
};

}

#endif
