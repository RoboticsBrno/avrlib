#ifndef AVRLIB_USART_XMEGA_HPP
#define AVRLIB_USART_XMEGA_HPP

#include <avr/io.h>
#include <stdint.h>

namespace avrlib {
	
enum uart_data_bits_t
{
	uart_5_bits = USART_CHSIZE_5BIT_gc,
	uart_6_bits = USART_CHSIZE_6BIT_gc,
	uart_7_bits = USART_CHSIZE_7BIT_gc,
	uart_8_bits = USART_CHSIZE_8BIT_gc,
	uart_9_bits = USART_CHSIZE_9BIT_gc
};

enum uart_parity_t
{
	uart_no_parity = USART_PMODE_DISABLED_gc,
	uart_even_parity = USART_PMODE_EVEN_gc,
	uart_odd_parity = USART_PMODE_ODD_gc
};

class uart_xmega
{
public:
	typedef uint8_t value_type;

	uart_xmega()
		: m_p(0)
	{
	}

	void open(USART_t & p, uint16_t baudrate, bool rx_interrupt = false, bool synchronous = false,
		uart_data_bits_t data_bits = uart_8_bits, bool stopbit2 = false, uart_parity_t parity = uart_no_parity)
	{
		m_p = &p;
		this->set_speed(baudrate);
		if (rx_interrupt)
			m_p->CTRLA = USART_RXCINTLVL_MED_gc;
		else
			m_p->CTRLA = 0;
		m_p->CTRLC = (synchronous? USART_CMODE_SYNCHRONOUS_gc: USART_CMODE_ASYNCHRONOUS_gc)
			| (data_bits & USART_CHSIZE_gm) | (parity & USART_PMODE_gm)
			| (stopbit2<<USART_SBMODE_bp);
		m_p->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	}

	void close()
	{
		m_p->CTRLA = 0;
		m_p->CTRLB = 0;
		m_p->CTRLC = USART_CMODE_ASYNCHRONOUS_gc | (3<<USART_CHSIZE_gp);
	}
	
	bool is_open() const { return m_p->CTRLB != 0; }

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
	
	bool frame_error() const
	{
		return (m_p->STATUS & USART_FERR_bm) != 0;
	}
	
	bool parity_error() const
	{
		return (m_p->STATUS & USART_PERR_bm) != 0;
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
	
	bool transmitted()
	{
		if((m_p->STATUS & USART_TXCIF_bm) == 0)
			return false;
		m_p->STATUS = USART_TXCIF_bm;
		return true;
	}
	
	bool bit9() const
	{
		return (m_p->STATUS & USART_RXB8_bm) != 0;
	}
	
	void bit9(const bool& value)
	{
		if(value)
			m_p->CTRLB |= USART_TXB8_bm;
		else
			m_p->CTRLB &= ~USART_TXB8_bm;
	}
	
	uart_data_bits_t data_bits() const
	{
		return uart_data_bits_t(m_p->CTRLC & USART_CHSIZE_gm);
	}
	
private:
	USART_t * m_p;
};

}

#endif
