#ifndef AVRLIB_USART1_HPP
#define AVRLIB_USART1_HPP

#include <avr/io.h>
#include <stdint.h>

#include "usart_base.hpp"

namespace avrlib {

class usart1
{
public:
	typedef uint8_t value_type;

	void open(uint32_t speed, bool rx_interrupt)
	{
		uint16_t ubrr = detail::get_ubrr(speed);

		UBRR1H = ubrr >> 8;
		UBRR1L = ubrr & 0xFF;
		UCSR1A = (1<<U2X1);
		UCSR1B = (1<<RXEN1)|(1<<TXEN1);

		if (rx_interrupt)
			UCSR1B |= (1<<RXCIE1);
	}

	void open_sync_slave(bool rx_interrupt)
	{
		UCSR1C = (1<<UMSEL10)|(1<<UCSZ11)|(1<<UCSZ10);
		UCSR1B = (1<<RXEN1)|(1<<TXEN1);

		if (rx_interrupt)
			UCSR1B |= (1<<RXCIE1);
	}

	void close()
	{
		UCSR1B = 0;
	}

	void send(value_type v)
	{
		UDR1 = v;
	}

	bool overflow() const
	{
		return (UCSR1A & (1<<DOR1)) != 0;
	}

	value_type recv()
	{
		return UDR1;
	}

	bool rx_empty() const
	{
		return (UCSR1A & (1<<RXC1)) == 0;
	}

	bool tx_empty() const
	{
		return (UCSR1A & (1<<UDRE1)) != 0;
	}
};

}

#endif
