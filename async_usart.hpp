#ifndef AVRLIB_ASYNC_USART_HPP
#define AVRLIB_ASYNC_USART_HPP

#include <stdint.h>
#include "nobootseq.hpp"
#include "buffer.hpp"

namespace avrlib {

template <typename Usart, int RxBufferSize, int TxBufferSize, typename Bootseq = nobootseq, typename Overflow = uint32_t>
class async_usart
{
public:
	typedef Usart usart_type;
	typedef Overflow overflow_type;
	typedef typename usart_type::value_type value_type;

	typedef Bootseq bootseq_type;

	async_usart()
	{
	}

	template <typename T1>
	async_usart(T1 const & t1)
	{
		m_usart.open(t1);
	}

	template <typename T1, typename T2>
	async_usart(T1 const & t1, T2 const & t2)
	{
		m_usart.open(t1, t2);
	}

	bool empty() const
	{
		return m_rx_buffer.empty();
	}

	bool tx_empty() const
	{
		return m_tx_buffer.empty();
	}

	bool tx_ready() const
	{
		return !m_tx_buffer.full();
	}
	
	bool transmitted()
	{
		return m_usart.transmitted();
	}

	value_type read()
	{
		while (m_rx_buffer.empty())
		{
			cli();
			this->process_rx();
			sei();
		}
		
		value_type res = m_rx_buffer.top();
		m_rx_buffer.pop();
		return res;
	}

	uint8_t read_size() const
	{
		return m_rx_buffer.size();
	}

	void write(value_type v)
	{
		if(TxBufferSize != 0)
		{
			while (m_tx_buffer.full())
			{
				cli();
				this->process_tx();
				sei();
			}

			m_tx_buffer.push(v);
		}
		else
		{
			while(!m_usart.tx_empty())
			{
				cli();
				__asm__ volatile ("nop");
				sei();
			}
			m_usart.send(v);
		}		
	}
	
	void flush()
	{
		bool tx_empty = false;
		while (!tx_empty)
		{
			cli();
			this->process_tx();
			tx_empty = m_tx_buffer.empty();
			sei();
		}
	}

	void process_rx()
	{
		if (!m_usart.rx_empty())
			this->intr_rx();
	}

	bool intr_rx()
	{
		if(m_usart.frame_error())
		{
			m_usart.recv();
			return false;
		}
		value_type v = m_bootseq.check(m_usart.recv());
		m_rx_buffer.push(v);
		return true;
	}

	bool process_tx()
	{
		if (!m_tx_buffer.empty() && m_usart.tx_empty())
		{
			m_usart.send(m_tx_buffer.top());
			m_tx_buffer.pop();
			return true;
		}

		// TODO: flush the underlying port
		return false;
	}
	
	bool intr_tx()
	{
		if (!m_tx_buffer.empty())
		{
			m_usart.send(m_tx_buffer.top());
			m_tx_buffer.pop();
			return true;
		}
		return false;
	}

	bool tx_reserve(uint8_t size)
	{
		return TxBufferSize - m_tx_buffer.size() > size;
	}

	typedef buffer<value_type, RxBufferSize> rx_buffer_type;
	rx_buffer_type & rx_buffer() { return m_rx_buffer; }
		
	typedef buffer<value_type, TxBufferSize> tx_buffer_type;
	tx_buffer_type & tx_buffer() { return m_tx_buffer; }

	usart_type & usart() { return m_usart; }
	usart_type const & usart() const { return m_usart; }

private:
	usart_type m_usart;
	buffer<value_type, RxBufferSize> m_rx_buffer;
	buffer<value_type, TxBufferSize==0?1:TxBufferSize> m_tx_buffer;
	bootseq_type m_bootseq;
};

}

#endif
