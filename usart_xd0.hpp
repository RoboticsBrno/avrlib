#ifndef AVRLIB_USART_XD0_HPP
#define AVRLIB_USART_XD0_HPP

#include <avr/io.h>
#include <stdint.h>

#include "intr_prio.hpp"

namespace avrlib {

class usart_xd0
{
public:
	typedef uint8_t value_type;

	void open(uint16_t baudrate, bool rx_interrupt = false, bool synchronous = false)
	{
		this->set_speed(baudrate);
		if (rx_interrupt)
			USARTD0.CTRLA = USART_RXCINTLVL_MED_gc;
		else
			USARTD0.CTRLA = 0;
		USARTD0.CTRLC = (synchronous? USART_CMODE_SYNCHRONOUS_gc: USART_CMODE_ASYNCHRONOUS_gc)
			| (3<<USART_CHSIZE_gp);
		USARTD0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
		
		PORTD.DIRSET = (1<<3);
	}

	void close()
	{
		USARTD0.CTRLA = 0;
		USARTD0.CTRLB = 0;
		USARTD0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | (3<<USART_CHSIZE_gp);
	}

	void set_speed(uint16_t baudrate)
	{
		USARTD0.BAUDCTRLA = (uint8_t)(baudrate);
		USARTD0.BAUDCTRLB = (uint8_t)(baudrate >> 8);
	}

	void send(value_type v)
	{
		USARTD0.DATA = v;
	}

	void dre_intr(intr_prio_t prio)
	{
		USARTD0.CTRLA = (USARTD0.CTRLA & ~(USART_DREINTLVL_gm)) | (prio<<USART_DREINTLVL_gp);
	}

	void rx_intr(intr_prio_t prio)
	{
		USARTD0.CTRLA = (USARTD0.CTRLA & ~(USART_RXCINTLVL_gm)) | (prio<<USART_RXCINTLVL_gp);
	}

	bool overflow() const
	{
		return (USARTD0.STATUS & USART_BUFOVF_bm) != 0;
	}

	value_type recv()
	{
		return USARTD0.DATA;
	}

	bool rx_empty() const
	{
		return (USARTD0.STATUS & USART_RXCIF_bm) == 0;
	}

	bool tx_empty() const
	{
		return (USARTD0.STATUS & USART_DREIF_bm) != 0;
	}
	
	bool frame_error() const
	{
		return (USARTD0_STATUS & USART_FERR_bm) != 0;
	}
};

}

#endif
