#ifndef AVRLIB_USART_HPP
#define AVRLIB_USART_HPP

#include <avr/io.h>
#include <stdint.h>

#include "usart_base.hpp"
#include "intr_prio.hpp"

#ifndef UPE
#define UPE PE
#endif

namespace avrlib {

class usart
{
public:
	typedef uint8_t value_type;

	usart()
	{
	}
	
	usart(uint16_t ubrr, bool rx_interrupt)
	{
		this->open(ubrr, rx_interrupt);
	}

	usart(uint32_t speed, bool rx_interrupt = false)
	{
		this->open(speed, rx_interrupt);
	}

	~usart()
	{
		UCSRB = 0;
	}

	void open(uint32_t speed, bool rx_interrupt = false)
	{
		uint16_t ubrr = detail::get_ubrr(speed);
		this->open(ubrr, rx_interrupt);
	}
	
	void open(uint16_t ubrr, bool rx_interrupt = false)
	{
		UBRRH = ubrr >> 8;
		UBRRL = ubrr & 0xFF;
		UCSRA = (1<<U2X);
		UCSRB = (1<<RXEN)|(1<<TXEN);

		if (rx_interrupt)
			UCSRB |= (1<<RXCIE);
	}

	void rx_intr(intr_prio_t prio)
	{
		if (prio)
			UCSRB |= (1<<RXCIE);
		else
			UCSRB &= ~(1<<RXCIE);
	}

#ifdef UMSEL00
	void open_sync_slave(bool rx_interrupt)
	{
		UCSRC = (1<<UMSEL)|(1<<UCSZ1)|(1<<UCSZ0);
		UCSRB = (1<<RXEN)|(1<<TXEN);

		if (rx_interrupt)
			UCSRB |= (1<<RXCIE);
	}
#endif

	void close()
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
	
	bool frame_error() const
	{
		return (UCSRA & (1<<FE)) != 0;
	}
	
	bool parity_error() const
	{
		return (UCSRA & (1<<UPE)) != 0;
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
