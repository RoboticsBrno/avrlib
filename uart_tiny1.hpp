#ifndef AVRLIB_USART_TINY1_HPP
#define AVRLIB_USART_TINY1_HPP

#include <avr/io.h>
#include <stdint.h>

#define USART_CHSIZE_9BIT_gc USART_CHSIZE_9BITH_gc
#define USART_RXB8_bm 1
#define USART_TXB8_bm 1

#include "usart_base.hpp"

namespace avrlib {

class uart_tiny1
{
public:
	typedef uint8_t value_type;

	uart_tiny1()
		: m_p(0)
	{
	}

	void open(USART_t & p, uint16_t baudrate, bool rxc_interrupt = false, bool synchronous = false,
		uart_data_bits_t data_bits = uart_8_bits, bool stopbit2 = false, 
		uart_parity_t parity = uart_no_parity, bool dre_interrupt = false,
		bool txc_interrupt = false)
	{
		open(p,
			 baudrate,
			 rxc_interrupt ? uart_intr_lo : uart_intr_off,
			 dre_interrupt ? uart_intr_lo : uart_intr_off,
			 txc_interrupt ? uart_intr_lo : uart_intr_off,
			 synchronous,
			 data_bits,
			 stopbit2,
			 parity);
	}

	void open(USART_t & p,
			  uint16_t baudrate,
			  uart_interrupt_priority_t rxc_interrupt = uart_intr_off,
			  uart_interrupt_priority_t dre_interrupt = uart_intr_off,
			  uart_interrupt_priority_t txc_interrupt = uart_intr_off,
			  bool synchronous = false,
			  uart_data_bits_t data_bits = uart_8_bits,
			  bool stopbit2 = false,
			  uart_parity_t parity = uart_no_parity)
	{
		m_p = &p;
		this->set_speed(baudrate);

		m_rxc_intr = rxc_interrupt != uart_intr_off;
		m_txc_intr = txc_interrupt != uart_intr_off;
		m_dre_intr = dre_interrupt != uart_intr_off;

		m_p->CTRLC = (synchronous? USART_CMODE_SYNCHRONOUS_gc: USART_CMODE_ASYNCHRONOUS_gc)
			| (data_bits & USART_CHSIZE_gm) | (parity & USART_PMODE_gm)
			| (stopbit2<<USART_SBMODE_bp);
		m_p->CTRLB = USART_RXEN_bm | USART_TXEN_bm;

		uint8_t ctrla = 0;
		if (m_rxc_intr)
			ctrla |= USART_RXCIE_bm;
		if (m_txc_intr)
			ctrla |= USART_TXCIE_bm;
		if (m_dre_intr)
			ctrla |= USART_DREIE_bm;

		//m_p->CTRLA = (m_txc_intr<<USART_TXCIE_bm) | (m_rxc_intr<<USART_RXCIE_bm) | (m_dre_intr<<USART_DREIE_bm); // this does not work, I don't know why and have not time to investigate right now
		m_p->CTRLA = ctrla;
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
		m_p->BAUD = baudrate;
	}

	void send(value_type v)
	{
		m_p->TXDATAL = v;
	}

	bool overflow() const
	{
		return (m_p->RXDATAH & USART_BUFOVF_bm) != 0;
	}
	
	bool frame_error() const
	{
		return (m_p->RXDATAH & USART_FERR_bm) != 0;
	}
	
	bool parity_error() const
	{
		return (m_p->RXDATAH & USART_PERR_bm) != 0;
	}

	value_type recv()
	{
		return m_p->RXDATAL;
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
		return (m_p->RXDATAH & USART_RXB8_bm) != 0;
	}
	
	void bit9(const bool& value)
	{
		if(value)
			m_p->RXDATAH |= USART_TXB8_bm;
		else
			m_p->RXDATAH &= ~USART_TXB8_bm;
	}
	
	uart_data_bits_t data_bits() const
	{
		return uart_data_bits_t(m_p->CTRLC & USART_CHSIZE_gm);
	}
	
	void usart(USART_t & p)
	{
		m_p = &p;
	}
	
	USART_t* usart()
	{
		return m_p;
	}
	
	void rxc_interrupt(const uart_interrupt_priority_t& priority, const bool& use = true)
	{
		rxc_interrupt(priority != uart_intr_off, use);
	}

	void _rxc_interrupt(const bool& en)
	{
		if(en)
			m_p->CTRLA |= (1<<USART_RXCIE_bm);
		else
			m_p->CTRLA &= ~(1<<USART_RXCIE_bm);
	}

	void rxc_interrupt(const bool& en, const bool& use = true)
	{
		m_rxc_intr = en;
		if (use)
			_rxc_interrupt(en);
	}

	uart_interrupt_priority_t get_rxc_interrupt() const
	{
		return m_rxc_intr ? uart_intr_lo : uart_intr_off;
	}
	uart_interrupt_priority_t read_rxc_interrupt() const
	{
		return (m_p->CTRLA & (1<<USART_RXCIE_bm)) != 0 ? uart_intr_lo : uart_intr_off;
	}

	bool is_rxc_interrupt_enable() const { return read_rxc_interrupt() != uart_intr_off; }
	bool is_rxc_interrupt_set() const { return get_rxc_interrupt() != uart_intr_off; }

	void enable_rxc_interrupt(const bool& enable = true)
	{
		_rxc_interrupt(m_rxc_intr && enable);
	}

	void disable_rxc_interrupt()
	{
		_rxc_interrupt(false);
	}
	

	void txc_interrupt(const uart_interrupt_priority_t& priority, const bool& use = true)
	{
		txc_interrupt(priority != uart_intr_off, use);
	}

	void _txc_interrupt(const bool& en)
	{
		if(en)
			m_p->CTRLA |= (1<<USART_TXCIE_bm);
		else
			m_p->CTRLA &= ~(1<<USART_TXCIE_bm);
	}

	void txc_interrupt(const bool& en, const bool& use = true)
	{
		m_txc_intr = en;
		if (use)
			_txc_interrupt(en);
	}

	uart_interrupt_priority_t get_txc_interrupt() const
	{
		return m_txc_intr ? uart_intr_lo : uart_intr_off;
	}
	uart_interrupt_priority_t read_txc_interrupt() const
	{
		return (m_p->CTRLA & (1<<USART_TXCIE_bm)) != 0 ? uart_intr_lo : uart_intr_off;
	}

	bool is_txc_interrupt_enable() const { return read_txc_interrupt() != uart_intr_off; }
	bool is_txc_interrupt_set() const { return get_txc_interrupt() != uart_intr_off; }

	void enable_txc_interrupt(const bool& enable = true)
	{
		_txc_interrupt(enable);
	}

	void disable_txc_interrupt()
	{
		_txc_interrupt(false);
	}


	void dre_interrupt(const uart_interrupt_priority_t& priority, const bool& use = true)
	{
		dre_interrupt(priority != uart_intr_off, use);
	}

	void _dre_interrupt(const bool& en)
	{
		if(en)
			m_p->CTRLA |= (1<<USART_DREIE_bm);
		else
			m_p->CTRLA &= ~(1<<USART_DREIE_bm);
	}

	void dre_interrupt(const bool& en, const bool& use = true)
	{
		m_dre_intr = en;
		if (use)
			_dre_interrupt(en);
	}

	uart_interrupt_priority_t get_dre_interrupt() const
	{
		return m_dre_intr ? uart_intr_lo : uart_intr_off;
	}
	uart_interrupt_priority_t read_dre_interrupt() const
	{
		return (m_p->CTRLA & (1<<USART_DREIE_bm)) != 0 ? uart_intr_lo : uart_intr_off;
	}

	bool is_dre_interrupt_enable() const { return read_dre_interrupt() != uart_intr_off; }
	bool is_dre_interrupt_set() const { return get_dre_interrupt() != uart_intr_off; }

	void enable_dre_interrupt(const bool& enable = true)
	{
		_dre_interrupt(enable);
	}

	void disable_dre_interrupt()
	{
		_dre_interrupt(false);
	}

private:
	USART_t * m_p;
	bool m_rxc_intr;
	bool m_dre_intr;
	bool m_txc_intr;
};

}

#endif
