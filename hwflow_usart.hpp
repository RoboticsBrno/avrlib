#ifndef AVRLIB_HWFLOW_USART_HPP
#define AVRLIB_HWFLOW_USART_HPP

#include <stdint.h>
#include "buffer.hpp"
#include "intr_prio.hpp"

namespace avrlib {

template <typename Usart, int RxBufferSize, int TxBufferSize, intr_prio_t RxPrio, typename PinRtr, typename PinCts>
class hwflow_usart
{
public:
	typedef Usart usart_type;
	typedef typename usart_type::value_type value_type;

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

	value_type read()
	{
		while (m_rx_buffer.empty())
		{
		}
		
		value_type res = m_rx_buffer.top();
		m_rx_buffer.pop();

		if (m_usart.rx_empty())
			PinRtr::set_low();
		m_usart.rx_intr(RxPrio);
		return res;
	}

	uint8_t read_size() const
	{
		return m_rx_buffer.size();
	}

	uint8_t operator[](uint8_t index) const
	{
		return m_rx_buffer[index];
	}

	void write(value_type v)
	{
		while (m_tx_buffer.full())
			this->process_tx();

		m_tx_buffer.push(v);
	}

	void process_rx()
	{
		if (!m_usart.rx_empty())
			this->intr_rx();
	}

	void intr_rx()
	{
		value_type v = m_usart.recv();
		m_rx_buffer.push(v);

		if (m_rx_buffer.full())
		{
			PinRtr::set_high();
			m_usart.rx_intr(intr_disabled);
		}
	}

	void process_tx()
	{
		if (!m_tx_buffer.empty() && m_usart.tx_empty() && !PinCts::read())
		{
			m_usart.send(m_tx_buffer.top());
			m_tx_buffer.pop();
		}
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
	buffer<value_type, TxBufferSize> m_tx_buffer;
};


template <typename Usart, int RxBufferSize, int TxBufferSize, int RxEmptyLevel, int RxFullLevel, intr_prio_t RxPrio, typename PinRtr, typename PinCts, typename Bootseq = nobootseq>
class enhanced_hwflow_usart
	: public hwflow_usart<Usart, RxBufferSize, TxBufferSize, RxPrio, PinRtr, PinCts>
{
public:
	typedef Usart usart_type;
	typedef typename usart_type::value_type value_type;
	
	typedef Bootseq bootseq_type;

	template <typename T1>
	enhanced_hwflow_usart(T1 const & t1)
	{
		this->usart().open(t1);
	}

	template <typename T1, typename T2>
	enhanced_hwflow_usart(T1 const & t1, T2 const & t2)
	{
		this->usart().open(t1, t2);
	}

	value_type read()
	{
		while (this->rx_buffer().empty())
		{
		}
		
		value_type res = this->rx_buffer().top();
		this->rx_buffer().pop();

		if (this->rx_buffer().size() < RxEmptyLevel)
			PinRtr::set_low();
		this->usart().rx_intr(RxPrio);
		return res;
	}

	bool intr_rx()
	{
		if(this->usart().frame_error())
		{
			this->usart().recv();
			return false;
		}
		value_type v = m_bootseq.check(this->usart().recv());
		this->rx_buffer().push(v);

		if (this->rx_buffer().size() >= RxFullLevel)
		{
			PinRtr::set_high();
			this->usart().rx_intr(intr_disabled);
		}
		return true;
	}
	
	void flush()
	{
		bool tx_empty = false;
		while (!tx_empty)
		{
			cli();
			this->process_tx();
			tx_empty = this->tx_empty();
			sei();
		}
	}
private:
	bootseq_type m_bootseq;
};

}

#endif
