#ifndef AVRLIB_ASYNC_USART_HPP
#define AVRLIB_ASYNC_USART_HPP

#include <stdint.h>
#include "usart_base.hpp"
#include "bootseq.hpp"
#include "buffer.hpp"

namespace avrlib {

template <typename Usart, int RxBufferSize, int TxBufferSize, typename Bootseq = nobootseq>
class async_usart
{
public:
	typedef Usart usart_type;
	typedef typename usart_type::value_type value_type;

	typedef Bootseq bootseq_type;

	explicit async_usart(uint32_t speed)
		: m_usart(detail::get_ubrr(speed))
	{
	}

	bool empty() const
	{
		return m_rx_buffer.empty();
	}
	
	value_type read()
	{
		while (m_rx_buffer.empty())
		{
		}
		
		value_type res = m_rx_buffer.top();
		m_rx_buffer.pop();
		return res;
	}

	void write(value_type v)
	{
		if (!m_tx_buffer.full())
			m_tx_buffer.push(v);
	}
	
	void process()
	{
		if (!m_usart.rx_empty())
		{
			value_type v = m_bootseq.check(m_usart.recv());
			if (!m_rx_buffer.full())
				m_rx_buffer.push(v);
		}
		
		if (!m_tx_buffer.empty() && m_usart.tx_empty())
		{
			m_usart.send(m_tx_buffer.top());
			m_tx_buffer.pop();
		}
	}

private:
	usart_type m_usart;
	buffer<value_type, RxBufferSize> m_rx_buffer;
	buffer<value_type, TxBufferSize> m_tx_buffer;
	bootseq_type m_bootseq;
};

}

#endif
