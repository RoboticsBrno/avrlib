#ifndef AVRLIB_SYNC_USART_HPP
#define AVRLIB_SYNC_USART_HPP

#include <stdint.h>
#include "nobootseq.hpp"

namespace avrlib {

template <typename Usart, typename Bootseq = nobootseq>
class sync_usart
{
public:
	typedef Usart usart_type;
	typedef typename usart_type::value_type value_type;

	typedef Bootseq bootseq_type;

	sync_usart()
	{
	}

	template <typename T1>
	sync_usart(T1 const & t1)
	{
		m_usart.open(t1);
	}

	template <typename T1, typename T2>
	sync_usart(T1 const & t1, T2 const & t2)
	{
		m_usart.open(t1, t2);
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
	
	void process_rx()
	{
	}

	void process_tx()
	{
	}

private:
	usart_type m_usart;
	bootseq_type m_bootseq;
};

}

#endif
