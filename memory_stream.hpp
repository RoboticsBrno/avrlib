#ifndef AVRLIB_MEMORY_STREAM_HPP
#define AVRLIB_MEMORY_STREAM_HPP

#include "numeric.hpp"

namespace avrlib {

template <uint_max_t RxCapacity, uint_max_t TxCapacity>
class memory_stream
{
public:
	memory_stream()
		: m_rx_pos(0), m_rx_size(0), m_tx_size(0)
	{
	}

	uint8_t * rx_buffer() { return m_rx_buffer; }
	uint8_t * tx_buffer() { return m_tx_buffer; }

	uint8_t rx_size() const { return m_rx_size - m_rx_pos; }
	uint8_t tx_size() const { return m_tx_size; }

	void rx_reset(uint8_t size) { m_rx_pos = 0; m_rx_size = size; }
	void rx_clear() { m_rx_pos = m_rx_size = 0; }
	void tx_clear() { m_tx_size = 0; }

	bool empty() const
	{
		return m_rx_pos == m_rx_size;
	}

	uint8_t read()
	{
		return m_rx_buffer[m_rx_pos++];
	}

	bool tx_ready() const
	{
		return m_tx_size != TxCapacity;
	}

	void write(uint8_t v)
	{
		if (m_tx_size != TxCapacity)
			m_tx_buffer[m_tx_size++] = v;
	}

private:
	uint8_t m_rx_buffer[RxCapacity];
	uint8_t m_tx_buffer[TxCapacity];
	uint8_t m_rx_pos;
	uint8_t m_rx_size;
	uint8_t m_tx_size;
};

} // namespace avrlib

#endif // AVRLIB_MEMORY_STREAM_HPP
