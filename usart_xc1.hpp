#ifndef AVRLIB_USART_XC1_HPP
#define AVRLIB_USART_XC1_HPP

#include <avr/io.h>
#include <stdint.h>

namespace avrlib {

class usart_xc1
{
public:
	typedef uint8_t value_type;

	void open(uint16_t baudrate, bool rx_interrupt = false, bool synchronous = false, bool dbl = false)
	{
		this->set_speed(baudrate);
		if (rx_interrupt)
			USARTC1.CTRLA = USART_RXCINTLVL_MED_gc;
		else
			USARTC1.CTRLA = 0;
		USARTC1.CTRLC = (synchronous? USART_CMODE_SYNCHRONOUS_gc: USART_CMODE_ASYNCHRONOUS_gc)
			| (3<<USART_CHSIZE_gp);
		USARTC1.CTRLB = USART_RXEN_bm | USART_TXEN_bm | (dbl? USART_CLK2X_bm: 0);
		
		PORTC.DIRSET = (1<<7);
	}

	void close()
	{
		USARTC1.CTRLA = 0;
		USARTC1.CTRLB = 0;
		USARTC1.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | (3<<USART_CHSIZE_gp);
	}

	void set_speed(uint16_t baudrate)
	{
		USARTC1.BAUDCTRLA = (uint8_t)(baudrate);
		USARTC1.BAUDCTRLB = (uint8_t)(baudrate >> 8);
	}

	void send(value_type v)
	{
		USARTC1.DATA = v;
	}

	bool overflow() const
	{
		return (USARTC1.STATUS & USART_BUFOVF_bm) != 0;
	}

	value_type recv()
	{
		return USARTC1.DATA;
	}

	bool rx_empty() const
	{
		return (USARTC1.STATUS & USART_RXCIF_bm) == 0;
	}

	bool tx_empty() const
	{
		return (USARTC1.STATUS & USART_DREIF_bm) != 0;
	}

	bool frame_error() const
	{
		return (USARTC1_STATUS & USART_FERR_bm) != 0;
	}
};

}

#endif
