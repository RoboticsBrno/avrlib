#ifndef AVRLIB_USART_XD1_HPP
#define AVRLIB_USART_XD1_HPP

#include <avr/io.h>
#include <stdint.h>

namespace avrlib {

class usart_xd1
{
public:
	typedef uint8_t value_type;

	// FIXME: ubrr is ignored (and does not make sense on xmega).
	usart_xd1(uint16_t ubrr, bool rx_interrupt)
	{
		PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTD.OUTSET = (1<<7);
		PORTD.DIRSET = (1<<7);

		USARTD1.BAUDCTRLA = 102;
		USARTD1.BAUDCTRLB = (-1<<USART_BSCALE_gp);
		if (rx_interrupt)
			USARTD1.CTRLA = USART_RXCINTLVL_MED_gc;
		else
			USARTD1.CTRLA = 0;
		USARTD1.CTRLC = (3<<USART_CHSIZE_gp);
		USARTD1.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	}

	~usart_xd1()
	{
		USARTD1.CTRLB = 0;
	}

	void send(value_type v)
	{
		USARTD1.DATA = v;
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
