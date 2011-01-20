#ifndef AVRLIB_USART_HPP
#define AVRLIB_USART_HPP

#include <avr/io.h>
#include <stdint.h>

namespace avrlib {

class usart
{
public:
	typedef uint8_t value_type;

	usart(uint16_t ubrr, bool rx_interrupt)
	{
		UBRRH = ubrr >> 8;
		UBRRL = ubrr & 0xFF;
		UCSRA = (1<<U2X);
#if defined(URSEL)
		UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
#else
		UCSRC = (1<<UCSZ1)|(1<<UCSZ0);
#endif
		UCSRB = (1<<RXEN)|(1<<TXEN);

		if (rx_interrupt)
			UCSRB |= (1<<RXCIE);
	}

	~usart()
	{
		UCSRB = 0;
	}

	void send(value_type v)
	{
		UDR = v;
	}

	bool overflow() const
	{
		return (UCSRA & (1<<DOR)) != 0;
	}

	value_type recv()
	{
		return UDR;
	}

	bool rx_empty() const
	{
		return (UCSRA & (1<<RXC)) == 0;
	}

	bool tx_empty() const
	{
		return (UCSRA & (1<<UDRE)) != 0;
	}
};

}

#endif
