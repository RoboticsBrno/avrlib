#ifndef AVRLIB_USART_XD1_HPP
#define AVRLIB_USART_XD1_HPP

#include <avr/io.h>
#include <stdint.h>

#include "intr_prio.hpp"

namespace avrlib {

class usart_xd1
{
public:
	typedef uint8_t value_type;

	void open(uint16_t baudrate, bool rx_interrupt = false, bool synchronous = false)
	{
		this->set_speed(baudrate);
		if (rx_interrupt)
			USARTD1.CTRLA = USART_RXCINTLVL_MED_gc;
		else
			USARTD1.CTRLA = 0;
		USARTD1.CTRLC = (synchronous? USART_CMODE_SYNCHRONOUS_gc: USART_CMODE_ASYNCHRONOUS_gc)
			| (3<<USART_CHSIZE_gp);
		USARTD1.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	}

	void close()
	{
		USARTD1.CTRLA = 0;
		USARTD1.CTRLB = 0;
		USARTD1.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | (3<<USART_CHSIZE_gp);
	}

	void set_speed(uint16_t baudrate)
	{
		USARTD1.BAUDCTRLA = (uint8_t)(baudrate);
		USARTD1.BAUDCTRLB = (uint8_t)(baudrate >> 8);
	}

	void send(value_type v)
	{
		USARTD1.DATA = v;
	}

	void dre_intr(intr_prio_t prio)
	{
		USARTD1.CTRLA = (USARTD1.CTRLA & ~(USART_DREINTLVL_gm)) | (prio<<USART_DREINTLVL_gp);
	}

	bool overflow() const
	{
		return (USARTD1.STATUS & USART_BUFOVF_bm) != 0;
	}

	value_type recv()
	{
		return USARTD1.DATA;
	}

	bool rx_empty() const
	{
		return (USARTD1.STATUS & USART_RXCIF_bm) == 0;
	}

	bool tx_empty() const
	{
		return (USARTD1.STATUS & USART_DREIF_bm) != 0;
	}
};

}

#endif
