#ifndef AVRLIB_SYNC_USART_HPP
#define AVRLIB_SYNC_USART_HPP

#include <stdint.h>
#include "usart_base.hpp"

namespace avrlib {

template <typename Usart, typename Bootseq = nobootseq>
class sync_usart
{
public:
	typedef Usart usart_type;
	typedef typename usart_type::value_type value_type;

	typedef Bootseq bootseq_type;

	explicit sync_usart(uint32_t speed)
		: m_usart(detail::get_ubrr(speed))
	{
	}

	bool empty() const
	{
		return m_usart.rx_empty();
	}
	
	value_type read()
	{
		while (m_usart.rx_empty())
		{
		}

		return m_bootseq.check(m_usart.recv());
	}

	void write(value_type v)
	{
		while (!m_usart.tx_empty())
		{
		}

		m_usart.send(v);
	}

private:
	usart_type m_usart;
	bootseq_type m_bootseq;
};

}

#endif
