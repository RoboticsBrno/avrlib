#ifndef AVRLIB_USART_XE0_HPP
#define AVRLIB_USART_XE0_HPP

#include <avr/io.h>
#include <stdint.h>

namespace avrlib {

class usart_xe0
{
public:
	typedef uint8_t value_type;

	void open(uint16_t baudrate, bool rx_interrupt = false, bool synchronous = false)
	{
		this->set_speed(baudrate);
		if (rx_interrupt)
			USARTE0.CTRLA = USART_RXCINTLVL_MED_gc;
		else
			USARTE0.CTRLA = 0;
		USARTE0.CTRLC = (synchronous? USART_CMODE_SYNCHRONOUS_gc: USART_CMODE_ASYNCHRONOUS_gc)
			| (3<<USART_CHSIZE_gp);
		USARTE0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	}

	void close()
	{
		USARTE0.CTRLA = 0;
		USARTE0.CTRLB = 0;
		USARTE0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | (3<<USART_CHSIZE_gp);
	}

	void set_speed(uint16_t baudrate)
	{
		USARTE0.BAUDCTRLA = (uint8_t)(baudrate);
		USARTE0.BAUDCTRLB = (uint8_t)(baudrate >> 8);
	}

	void send(value_type v)
	{
		USARTE0.DATA = v;
	}

	bool overflow() const
	{
		return (USARTE0.STATUS & USART_BUFOVF_bm) != 0;
	}

	value_type recv()
	{
		return USARTE0.DATA;
	}

	bool rx_empty() const
	{
		return (USARTE0.STATUS & USART_RXCIF_bm) == 0;
	}

	bool tx_empty() const
	{
		return (USARTE0.STATUS & USART_DREIF_bm) != 0;
	}
};

}

#endif
