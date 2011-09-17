#ifndef AVRLIB_USART_XD1_HPP
#define AVRLIB_USART_XD1_HPP

#include <avr/io.h>
#include <stdint.h>

namespace avrlib {

class uart_xmega
{
public:
	typedef uint8_t value_type;

	uart_xmega()
		: m_p(0)
	{
	}

	void open(USART_t & p, bool rx_interrupt = false)
	{
		m_p = &p;
		m_p->BAUDCTRLA = 102;
		m_p->BAUDCTRLB = (-1<<USART_BSCALE_gp);
		if (rx_interrupt)
			m_p->CTRLA = USART_RXCINTLVL_MED_gc;
		else
			m_p->CTRLA = 0;
		m_p->CTRLC = (3<<USART_CHSIZE_gp);
		m_p->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	}

	void close()
	{
		m_p->CTRLB = 0;
	}

	void send(value_type v)
	{
		m_p->DATA = v;
	}

	bool overflow() const
	{
		return (m_p->STATUS & USART_BUFOVF_bm) != 0;
	}

	value_type recv()
	{
		return m_p->DATA;
	}

	bool rx_empty() const
	{
		return (m_p->STATUS & USART_RXCIF_bm) == 0;
	}

	bool tx_empty() const
	{
		return (m_p->STATUS & USART_DREIF_bm) != 0;
	}
	
private:
	USART_t * m_p;
};

}

#endif
