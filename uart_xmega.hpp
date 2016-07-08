#ifndef AVRLIB_USART_XMEGA_HPP
#define AVRLIB_USART_XMEGA_HPP

#include <avr/io.h>
#include <stdint.h>

#include "usart_base.hpp"

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

#ifndef AVRLIB_USART_XMEGA_DEFAULT_INTERRUPT_PRIORITY
#define AVRLIB_USART_XMEGA_DEFAULT_INTERRUPT_PRIORITY c_default_interrupt_priority
#endif

class uart_xmega
{
public:
	typedef uint8_t value_type;

	const uart_interrupt_priority_t c_default_interrupt_priority = AVRLIB_USART_XMEGA_DEFAULT_INTERRUPT_PRIORITY;

	uart_xmega()
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
			 rxc_interrupt ? c_default_interrupt_priority : uart_intr_off,
			 dre_interrupt ? c_default_interrupt_priority : uart_intr_off,
			 txc_interrupt ? c_default_interrupt_priority : uart_intr_off,
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

		m_txc_intr_prior = USART_TXCINTLVL_t(txc_interrupt<<USART_TXCINTLVL_gp);
		m_rxc_intr_prior = USART_RXCINTLVL_t(rxc_interrupt<<USART_RXCINTLVL_gp);
		m_dre_intr_prior = USART_DREINTLVL_t(dre_interrupt<<USART_DREINTLVL_gp);

		m_p->CTRLC = (synchronous? USART_CMODE_SYNCHRONOUS_gc: USART_CMODE_ASYNCHRONOUS_gc)
			| (data_bits & USART_CHSIZE_gm) | (parity & USART_PMODE_gm)
			| (stopbit2<<USART_SBMODE_bp);
		m_p->CTRLB = USART_RXEN_bm | USART_TXEN_bm;

		m_p->CTRLA = m_txc_intr_prior | m_rxc_intr_prior | m_dre_intr_prior;
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
	
	void usart(USART_t & p)
	{
		m_p = &p;
	}
	
	USART_t* usart()
	{
		return m_p;
	}
	
	void rxc_interrupt(const bool& en, const bool& use = true)
	{
		rxc_interrupt(en ? c_default_interrupt_priority : uart_intr_off, use);
	}
	void rxc_interrupt(const uart_interrupt_priority_t& priority, const bool& use = true)
	{
		m_rxc_intr_prior = USART_RXCINTLVL_t(priority<<USART_RXCINTLVL_gp);
		if(use)
			m_p->CTRLA = (m_p->CTRLA & ~USART_RXCINTLVL_gm) | m_rxc_intr_prior;
	}

	uart_interrupt_priority_t get_rxc_interrupt() const
	{
		return uart_interrupt_priority_t(m_rxc_intr_prior>>USART_RXCINTLVL_gp);
	}
	uart_interrupt_priority_t read_rxc_interrupt() const
	{
		return uart_interrupt_priority_t((m_p->CTRLA & USART_RXCINTLVL_gm)>>USART_RXCINTLVL_gp);
	}
	
	bool rxc_interrupt() const { return read_rxc_interrupt() != uart_intr_off; } // deprecated
	bool is_rxc_interrupt_enable() const { return read_rxc_interrupt() != uart_intr_off; }
	bool is_rxc_interrupt_set() const { return get_rxc_interrupt() != uart_intr_off; }

	void enable_rxc_interrupt(const bool& enable = true)
	{
		m_p->CTRLA = (m_p->CTRLA & (~USART_RXCINTLVL_gm)) | (enable ? m_rxc_intr_prior : USART_RXCINTLVL_OFF_gc);
	}

	void disable_rxc_interrupt()
	{
		enable_rxc_interrupt(false);
	}

	void txc_interrupt(const bool& en, const bool& use = true)
	{
		txc_interrupt(en ? c_default_interrupt_priority : uart_intr_off, use);
	}
	void txc_interrupt(const uart_interrupt_priority_t& priority, const bool& use = true)
	{
		m_txc_intr_prior = USART_TXCINTLVL_t(priority<<USART_TXCINTLVL_gp);
		if(use)
			m_p->CTRLA = (m_p->CTRLA & ~USART_TXCINTLVL_gm) | m_txc_intr_prior;
	}

	uart_interrupt_priority_t get_txc_interrupt() const
	{
		return uart_interrupt_priority_t(m_txc_intr_prior>>USART_TXCINTLVL_gp);
	}
	uart_interrupt_priority_t read_txc_interrupt() const
	{
		return uart_interrupt_priority_t((m_p->CTRLA & USART_TXCINTLVL_gm)>>USART_TXCINTLVL_gp);
	}
	
	bool tx_interrupt() const { return read_txc_interrupt() != uart_intr_off; } // deprecated
	bool is_txc_interrupt_enable() const { return read_txc_interrupt() != uart_intr_off; }
	bool is_txc_interrupt_set() const { return get_txc_interrupt() != uart_intr_off; }

	void enable_txc_interrupt(const bool& enable = true)
	{
		m_p->CTRLA = (m_p->CTRLA & (~USART_TXCINTLVL_gm)) | (enable ? m_txc_intr_prior : USART_TXCINTLVL_OFF_gc);
	}

	void disable_txc_interrupt()
	{
		enable_txc_interrupt(false);
	}

	void dre_interrupt(const bool& en, const bool& use = true)
	{
		dre_interrupt(en ? c_default_interrupt_priority : uart_intr_off, use);
	}
	void dre_interrupt(const uart_interrupt_priority_t& priority, const bool& use = true)
	{
		m_dre_intr_prior = USART_DREINTLVL_t(priority<<USART_DREINTLVL_gp);
		if(use)
			m_p->CTRLA = (m_p->CTRLA & ~USART_DREINTLVL_gm) | m_dre_intr_prior;
	}

	uart_interrupt_priority_t get_dre_interrupt() const
	{
		return uart_interrupt_priority_t(m_dre_intr_prior>>USART_DREINTLVL_gp);
	}
	uart_interrupt_priority_t read_dre_interrupt() const
	{
		return uart_interrupt_priority_t((m_p->CTRLA & USART_DREINTLVL_gm)>>USART_DREINTLVL_gp);
	}

	bool dre_interrupt() const { return read_dre_interrupt() != uart_intr_off; } // deprecated
	bool is_dre_interrupt_enabled() const {return read_dre_interrupt() != uart_intr_off; }
	bool is_dre_interrupt_set() const {return get_dre_interrupt() != uart_intr_off; }
	
	void enable_dre_interrupt(const bool& enable = true)
	{
		m_p->CTRLA = (m_p->CTRLA & (~USART_DREINTLVL_gm)) | (enable ? m_dre_intr_prior : USART_DREINTLVL_OFF_gc);
	}

	void disable_dre_interrupt()
	{
		enable_dre_interrupt(false);
	}

private:
	USART_t * m_p;
	USART_RXCINTLVL_t m_rxc_intr_prior;
	USART_DREINTLVL_t m_dre_intr_prior;
	USART_TXCINTLVL_t m_txc_intr_prior;
};

}

#endif
