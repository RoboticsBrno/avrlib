#ifndef AVRLIB_USART0_HPP
#define AVRLIB_USART0_HPP

#include <avr/io.h>
#include <stdint.h>

#include "usart_base.hpp"
#include "intr_prio.hpp"

#ifndef FE0
#define FE0 FE
#endif
#ifndef UPE0
#define UPE0 UPE
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
	uart_no_parity   = (0<<UPM01) | (0<<UPM00),
	uart_even_parity = (1<<UPM01) | (0<<UPM00),
	uart_odd_parity  = (1<<UPM01) | (1<<UPM00)
};

class usart0
{
public:
	typedef uint8_t value_type;

	usart0()
	{
	}

	usart0(uint16_t ubrr, bool rx_interrupt = false)
	{
		this->open(ubrr, rx_interrupt);
	}
	
	usart0(uint32_t speed, bool rx_interrupt = false)
	{
		this->open(detail::get_ubrr(speed), rx_interrupt);
	}

	~usart0()
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
		
		UBRR0H = baudrate >> 8;
		UBRR0L = baudrate & 0xFF;
		UCSR0A = (1<<U2X0);
		UCSR0B = (1<<RXEN0)
			   | (1<<TXEN0)
			   | (rxc_interrupt<<RXCIE0)
			   | (txc_interrupt<<TXCIE0)
			   | (dre_interrupt<<UDRIE0)
			   | (data_bits == uart_9_bits ? (1<<UCSZ02) : 0);
		UCSR0C = (synchronous<<UMSEL00)
			   | (parity & ((1<<UPM01) | (1<<UPM00)))
			   | (stopbit2<<USBS0)
			   | ((data_bits & 3)<<UCSZ00);
	}

	void close()
	{
		UCSR0A = 0;
		UCSR0B = 0;
		UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
		UBRR0H = 0;
		UBRR0L = 0;
	}

	bool is_open() const { return (UCSR0B & ((1<<RXEN0) | (1<<TXEN0))) != 0; }

	void set_speed(uint16_t baudrate)
	{
		UBRR0H = baudrate >> 8;
		UBRR0L = baudrate & 0xFF;
	}

#ifdef UMSEL00
	void open_sync_slave(bool rx_interrupt)
	{
		UCSR0C = (1<<UMSEL00)|(1<<UCSZ01)|(1<<UCSZ00);
		UCSR0B = (1<<RXEN0)|(1<<TXEN0);
		m_rxc_intr = rx_interrupt;
		if (rx_interrupt)
			UCSR0B |= (1<<RXCIE0);
	}
#endif

	void send(value_type v)
	{
		UDR0 = v;
	}

	bool overflow() const
	{
		return (UCSR0A & (1<<DOR0)) != 0;
	}
	
	bool frame_error() const
	{
		return (UCSR0A & (1<<FE0)) != 0;
	}
	
	bool parity_error() const
	{
		return (UCSR0A & (1<<UPE0)) != 0;
	}

	value_type recv()
	{
		return UDR0;
	}

	bool rx_empty() const
	{
		return (UCSR0A & (1<<RXC0)) == 0;
	}

	bool tx_empty() const
	{
		return (UCSR0A & (1<<UDRE0)) != 0;
	}
	
	bool transmitted()
	{
		if((UCSR0A & (1<<TXC0)) == 0)
			return false;
		UCSR0A |= (1<<TXC0);
		return true;
	}
	
	bool bit9() const
	{
		return (UCSR0B & (1<<RXB80)) != 0;
	}
	
	void bit9(const bool& value)
	{
		if(value)
			UCSR0B |= (1<<TXB80);
		else
			UCSR0B &= ~(1<<TXB80);
	}
	
	uart_data_bits_t data_bits() const
	{
		return uart_data_bits_t(
			(((UCSR0B & (1<<UCSZ02))>>UCSZ02)<<2) |
			(((UCSR0C & (1<<UCSZ01))>>UCSZ01)<<1) |
			(((UCSR0C & (1<<UCSZ00))>>UCSZ00)<<0)
		);
	}
	
	void rxc_interrupt(const uart_interrupt_priority_t& priority, const bool& use = true)
	{
		rxc_interrupt(priority != uart_intr_off, use);
	}

	void _rxc_interrupt(const bool& en)
	{
		if(en)
			UCSR0B |= (1<<RXCIE0);
		else
			UCSR0B &= ~(1<<RXCIE0);
	}

	void rxc_interrupt(const bool& en, const bool& use = true)
	{
		m_rxc_intr = en;
		if (!use)
			return;
	}
	
	bool rxc_interrupt() const { return (UCSR0B & (1<<RXCIE0)) != 0; } // deprecated

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
			UCSR0B |= (1<<TXCIE0);
		else
			UCSR0B &= ~(1<<TXCIE0);
	}

	void txc_interrupt(const bool& en, const bool& use = true)
	{
		m_txc_intr = en;
		if (use)
			_txc_interrupt(en);
	}
	
	bool txc_interrupt() const { return (UCSR0B & (1<<TXCIE0)) != 0; } // deprecated

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
		_txc_interrupt(m_txc_intr && enable);
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
			UCSR0B |= (1<<UDRIE0);
		else
			UCSR0B &= ~(1<<UDRIE0);
	}

	void dre_interrupt(const bool& en, const bool& use = true)
	{
		m_dre_intr = en;
		if (use)
			_dre_interrupt(en);
	}
	
	bool dre_interrupt() const { return (UCSR0B & (1<<UDRIE0)) != 0; } // deprecated

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
		_dre_interrupt(m_dre_intr && enable);
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
