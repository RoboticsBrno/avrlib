#ifndef AVRLIB_USART0_HPP
#define AVRLIB_USART0_HPP

#include <avr/io.h>
#include <stdint.h>

#include "usart_base.hpp"
#include "intr_prio.hpp"

namespace avrlib {

class usart0
{
public:
	typedef uint8_t value_type;

	usart0()
	{
	}

	usart0(uint16_t ubrr, bool rx_interrupt = false)
	{
		this->open(ubrr, rx_interrupt);
	}
	
	usart0(uint32_t speed, bool rx_interrupt = false)
	{
		this->open(speed, rx_interrupt);
	}

	~usart0()
	{
		this->close();
	}

	void open(uint32_t speed, bool rx_interrupt = false)
	{
		uint16_t ubrr = detail::get_ubrr(speed);
		this->open(ubrr, rx_interrupt);
	}
	
	void open(uint16_t ubrr, bool rx_interrupt = false)
	{
		UBRR0H = ubrr >> 8;
		UBRR0L = ubrr & 0xFF;
		UCSR0A = (1<<U2X0);
		UCSR0B = (1<<RXEN0)|(1<<TXEN0);

		if (rx_interrupt)
			UCSR0B |= (1<<RXCIE0);
	}
	
	void rx_intr(intr_prio_t prio)
	{
		if (prio)
			UCSR0B |= (1<<RXCIE0);
		else
			UCSR0B &= ~(1<<RXCIE0);
	}

#ifdef UMSEL00
	void open_sync_slave(bool rx_interrupt)
	{
		UCSR0C = (1<<UMSEL00)|(1<<UCSZ01)|(1<<UCSZ00);
		UCSR0B = (1<<RXEN0)|(1<<TXEN0);

		if (rx_interrupt)
			UCSR0B |= (1<<RXCIE0);
	}
#endif

	void close()
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
	
	bool frame_error() const
	{
		return (UCSR0A & (1<<FE)) != 0;
	}
	
	bool parity_error() const
	{
		return (UCSR0A & (1<<UPE)) != 0;
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
