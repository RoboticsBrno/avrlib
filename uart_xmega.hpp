#ifndef AVRLIB_USART_XMEGA_HPP
#define AVRLIB_USART_XMEGA_HPP

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

	void open(USART_t & p, uint16_t baudrate, bool rx_interrupt = false, bool synchronous = false)
	{
		m_p = &p;
		this->set_speed(baudrate);
		if (rx_interrupt)
			m_p->CTRLA = USART_RXCINTLVL_MED_gc;
		else
			m_p->CTRLA = 0;
		m_p->CTRLC = (synchronous? USART_CMODE_SYNCHRONOUS_gc: USART_CMODE_ASYNCHRONOUS_gc)
			| (3<<USART_CHSIZE_gp);
		m_p->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	}

	void close()
	{
		m_p->CTRLA = 0;
		m_p->CTRLB = 0;
		m_p->CTRLC = USART_CMODE_ASYNCHRONOUS_gc | (3<<USART_CHSIZE_gp);
	}

	void set_speed(uint16_t baudrate)
	{
		m_p->BAUDCTRLA = (uint8_t)(baudrate);
		m_p->BAUDCTRLB = (uint8_t)(baudrate >> 8);
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
