#ifndef AVRLIB_USART1_HPP
#define AVRLIB_USART1_HPP

#include <avr/io.h>
#include <stdint.h>

#include "usart_base.hpp"

#ifndef FE1
#define FE1 FE
#endif
#ifndef UPE1
#define UPE1 UPE
#endif
#ifndef UMSEL10
#define UMSEL10 UMSEL1
#endif

namespace avrlib {

class usart1
{
public:
	typedef uint8_t value_type;

	usart1()
	{
	}

	usart1(uint16_t ubrr, bool rx_interrupt = false)
	{
		this->open(ubrr, rx_interrupt);
	}
	
	usart1(uint32_t speed, bool rx_interrupt = false)
	{
		this->open(detail::get_ubrr(speed), rx_interrupt);
	}

	~usart1()
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
		
		UBRR1H = baudrate >> 8;
		UBRR1L = baudrate & 0xFF;
		UCSR1A = (1<<U2X1);
		UCSR1B = (1<<RXEN1)
			   | (1<<TXEN1)
			   | (rxc_interrupt<<RXCIE1)
			   | (txc_interrupt<<TXCIE1)
			   | (dre_interrupt<<UDRIE1)
			   | (data_bits == uart_9_bits ? (1<<UCSZ12) : 0);
		UCSR1C = (synchronous<<UMSEL10)
			   | (parity & ((1<<UPM11) | (1<<UPM10)))
			   | (stopbit2<<USBS1)
			   | ((data_bits & 3)<<UCSZ10);
	}

	void close()
	{
		UCSR1A = 0;
		UCSR1B = 0;
		UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
		UBRR1H = 0;
		UBRR1L = 0;
	}

	bool is_open() const { return (UCSR1B & ((1<<RXEN1) | (1<<TXEN1))) != 0; }

	void set_speed(uint16_t baudrate)
	{
		UBRR1H = baudrate >> 8;
		UBRR1L = baudrate & 0xFF;
	}

#ifdef UMSEL10
	void open_sync_slave(bool rx_interrupt)
	{
		UCSR1C = (1<<UMSEL10)|(1<<UCSZ11)|(1<<UCSZ10);
		UCSR1B = (1<<RXEN1)|(1<<TXEN1);
		m_rxc_intr = rx_interrupt;
		if (rx_interrupt)
			UCSR1B |= (1<<RXCIE1);
	}
#endif

	void send(value_type v)
	{
		UDR1 = v;
	}

	bool overflow() const
	{
		return (UCSR1A & (1<<DOR1)) != 0;
	}
	
	bool frame_error() const
	{
		return (UCSR1A & (1<<FE1)) != 0;
	}
	
	bool parity_error() const
	{
		return (UCSR1A & (1<<UPE1)) != 0;
	}

	value_type recv()
	{
		return UDR1;
	}

	bool rx_empty() const
	{
		return (UCSR1A & (1<<RXC1)) == 0;
	}

	bool tx_empty() const
	{
		return (UCSR1A & (1<<UDRE1)) != 0;
	}
	
	bool transmitted()
	{
		if((UCSR1A & (1<<TXC1)) == 0)
			return false;
		UCSR1A |= (1<<TXC1);
		return true;
	}
	
	bool bit9() const
	{
		return (UCSR1B & (1<<RXB81)) != 0;
	}
	
	void bit9(const bool& value)
	{
		if(value)
			UCSR1B |= (1<<TXB81);
		else
			UCSR1B &= ~(1<<TXB81);
	}
	
	uart_data_bits_t data_bits() const
	{
		return uart_data_bits_t(
			(((UCSR1B & (1<<UCSZ12))>>UCSZ12)<<2) |
			(((UCSR1C & (1<<UCSZ11))>>UCSZ11)<<1) |
			(((UCSR1C & (1<<UCSZ10))>>UCSZ10)<<0)
		);
	}
	
	void rxc_interrupt(const uart_interrupt_priority_t& priority, const bool& use = true)
	{
		rxc_interrupt(priority != uart_intr_off, use);
	}

	void _rxc_interrupt(const bool& en)
	{
		if(en)
			UCSR1B |= (1<<RXCIE1);
		else
			UCSR1B &= ~(1<<RXCIE1);
	}

	void rxc_interrupt(const bool& en, const bool& use = true)
	{
		m_rxc_intr = en;
		if (!use)
			return;
	}
	
	bool rxc_interrupt() const { return (UCSR1B & (1<<RXCIE1)) != 0; } // deprecated

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
			UCSR1B |= (1<<TXCIE1);
		else
			UCSR1B &= ~(1<<TXCIE1);
	}

	void txc_interrupt(const bool& en, const bool& use = true)
	{
		m_txc_intr = en;
		if (use)
			_txc_interrupt(en);
	}
	
	bool txc_interrupt() const { return (UCSR1B & (1<<TXCIE1)) != 0; } // deprecated

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
			UCSR1B |= (1<<UDRIE1);
		else
			UCSR1B &= ~(1<<UDRIE1);
	}

	void dre_interrupt(const bool& en, const bool& use = true)
	{
		m_dre_intr = en;
		if (use)
			_dre_interrupt(en);
	}
	
	bool dre_interrupt() const { return (UCSR1B & (1<<UDRIE1)) != 0; } // deprecated

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
