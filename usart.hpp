#ifndef AVRLIB_USART_HPP
#define AVRLIB_USART_HPP

#include <avr/io.h>
#include <stdint.h>

#include "usart_base.hpp"
#include "intr_prio.hpp"

#ifndef UPE
#define UPE PE
#endif

namespace avrlib {

enum uart_data_bits_t
{
	uart_5_bits = 0,
	uart_6_bits = 1,
	uart_7_bits = 2,
	uart_8_bits = 3,
	uart_9_bits = 7
};

enum uart_parity_t
{
	uart_no_parity   = (0<<UPM1) | (0<<UPM0),
	uart_even_parity = (1<<UPM1) | (0<<UPM0),
	uart_odd_parity  = (1<<UPM1) | (1<<UPM0)
};

class usart
{
public:
	typedef uint8_t value_type;

	usart()
	{
	}

	usart(uint16_t ubrr, bool rx_interrupt = false)
	{
		this->open(ubrr, rx_interrupt);
	}
	
	usart(uint32_t speed, bool rx_interrupt = false)
	{
		this->open(detail::get_ubrr(speed), rx_interrupt);
	}

	~usart()
	{
		close();
	}


	void open(uint16_t baudrate,
			  uart_interrupt_priority_t rxc_interrupt = uart_intr_off,
			  uart_interrupt_priority_t dre_interrupt = uart_intr_off,
			  uart_interrupt_priority_t txc_interrupt = uart_intr_off,
			  bool synchronous = false,
			  uart_data_bits_t data_bits = uart_8_bits,
			  bool stopbit2 = false,
			  uart_parity_t parity = uart_no_parity)
	{
		open(baudrate,
			 rxc_interrupt != uart_intr_off,
			 synchronous,
			 data_bits,
			 stopbit2,
			 parity,
			 dre_interrupt != uart_intr_off,
			 txc_interrupt != uart_intr_off);
	}
	
	void open(uint16_t baudrate,
			  bool rxc_interrupt = false,
			  bool synchronous = false,
			  uart_data_bits_t data_bits = uart_8_bits,
			  bool stopbit2 = false, 
			  uart_parity_t parity = uart_no_parity,
			  bool dre_interrupt = false,
			  bool txc_interrupt = false)
	{
		m_txc_intr = m_txc_intr;
		m_rxc_intr = m_rxc_intr;
		m_dre_intr = m_dre_intr;

		UBRRH = baudrate >> 8;
		UBRRL = baudrate & 0xFF;
		UCSRA = (1<<U2X);
		UCSRB = (1<<RXEN)
			  | (1<<TXEN)
			  | (rxc_interrupt<<RXCIE)
			  | (txc_interrupt<<TXCIE)
			  | (dre_interrupt<<UDRIE)
			  | (data_bits == uart_9_bits ? (1<<UCSZ2) : 0);
		UCSRC = (1<<URSEL)
			  | (synchronous<<UMSEL)
			  | (parity & ((1<<UPM1) | (1<<UPM0)))
			  | (stopbit2<<USBS)
			  | ((data_bits & 3)<<UCSZ0);
	}

	void close()
	{
		UCSRA = 0;
		UCSRB = 0;
		UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
		UBRRH = 0;
		UBRRL = 0;
	}

	bool is_open() const { return (UCSRB & ((1<<RXEN) | (1<<TXEN))) != 0; }

	void set_speed(uint16_t baudrate)
	{
		UBRRH = baudrate >> 8;
		UBRRL = baudrate & 0xFF;
	}

#ifdef UMSEL00
	void open_sync_slave(bool rx_interrupt)
	{
		UCSRC = (1<<UMSEL)|(1<<UCSZ1)|(1<<UCSZ0);
		UCSRB = (1<<RXEN)|(1<<TXEN);
		m_rxc_intr = rx_interrupt;
		if (rx_interrupt)
			UCSRB |= (1<<RXCIE);
	}
#endif

	void send(value_type v)
	{
		UDR = v;
	}

	bool overflow() const
	{
		return (UCSRA & (1<<DOR)) != 0;
	}
	
	bool frame_error() const
	{
		return (UCSRA & (1<<FE)) != 0;
	}
	
	bool parity_error() const
	{
		return (UCSRA & (1<<UPE)) != 0;
	}

	value_type recv()
	{
		return UDR;
	}

	bool rx_empty() const
	{
		return (UCSRA & (1<<RXC)) == 0;
	}

	bool tx_empty() const
	{
		return (UCSRA & (1<<UDRE)) != 0;
	}

	bool transmitted()
	{
		if((UCSRA & (1<<TXC)) == 0)
			return false;
		UCSRA |= (1<<TXC);
		return true;
	}

	bool bit9() const
	{
		return (UCSRB & (1<<RXB8)) != 0;
	}
	
	void bit9(const bool& value)
	{
		if(value)
			UCSRB |= (1<<TXB8);
		else
			UCSRB &= ~(1<<TXB8);
	}
	
	uart_data_bits_t data_bits() const
	{
		return uart_data_bits_t(
			(((UCSRB & (1<<UCSZ2))>>UCSZ2)<<2) |
			(((UCSRC & (1<<UCSZ1))>>UCSZ1)<<1) |
			(((UCSRC & (1<<UCSZ0))>>UCSZ0)<<0)
		);
	}
	
	void rxc_interrupt(const uart_interrupt_priority_t& priority, const bool& use = true)
	{
		rxc_interrupt(priority != uart_intr_off, use);
	}

	void _rxc_interrupt(const bool& en)
	{
		if(en)
			UCSRB |= (1<<RXCIE);
		else
			UCSRB &= ~(1<<RXCIE);
	}

	void rxc_interrupt(const bool& en, const bool& use = true)
	{
		m_rxc_intr = en;
		if (use)
			_rxc_interrupt(en)
		
	}
	
	bool rxc_interrupt() const { return (UCSRB & (1<<RXCIE)) != 0; } // deprecated

	uart_interrupt_priority_t get_rxc_interrupt() const
	{
		return m_rxc_intr ? uart_intr_lo : uart_intr_off;
	}
	uart_interrupt_priority_t read_rxc_interrupt() const
	{
		return rxc_interrupt() ? uart_intr_lo : uart_intr_off;
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
			UCSRB |= (1<<TXCIE);
		else
			UCSRB &= ~(1<<TXCIE);
	}

	void txc_interrupt(const bool& en, const bool& use = true)
	{
		m_txc_intr = en;
		if (use)
			_txc_interrupt(en);
	}
	
	bool txc_interrupt() const { return (UCSRB & (1<<TXCIE)) != 0; } // deprecated

	uart_interrupt_priority_t get_txc_interrupt() const
	{
		return m_txc_intr ? uart_intr_lo : uart_intr_off;
	}
	uart_interrupt_priority_t read_txc_interrupt() const
	{
		return txc_interrupt() ? uart_intr_lo : uart_intr_off;
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
			UCSRB |= (1<<UDRIE);
		else
			UCSRB &= ~(1<<UDRIE);
	}

	void dre_interrupt(const bool& en, const bool& use = true)
	{
		m_dre_intr = en;
		if (use)
			_dre_interrupt(en);
	}
	
	bool dre_interrupt() const { return (UCSRB & (1<<UDRIE)) != 0; } // deprecated

	uart_interrupt_priority_t get_dre_interrupt() const
	{
		return m_dre_intr ? uart_intr_lo : uart_intr_off;
	}
	uart_interrupt_priority_t read_dre_interrupt() const
	{
		return dre_interrupt() ? uart_intr_lo : uart_intr_off;
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
	bool m_rxc_intr;
	bool m_dre_intr;
	bool m_txc_intr;
};

}

#endif
