#ifndef AVRLIB_BUFFER_HPP
#define AVRLIB_BUFFER_HPP

#include <stdint.h>

namespace avrlib {

template <typename T, uint8_t Capacity>
class buffer
{
public:
	typedef T value_type;
	static const uint8_t capacity = Capacity;

	buffer()
		: m_wptr(0), m_rptr(0)
	{
	}

	void clear()
	{
		m_rptr = m_wptr;
	}

	bool empty() const
	{
		return m_wptr == m_rptr;
	}

	bool full() const
	{
		return next(m_wptr) == m_rptr;
	}

	void push(value_type v)
	{
		m_buffer[m_wptr] = v;
		m_wptr = next(m_wptr);
	}

	value_type top() const
	{
		return m_buffer[m_rptr];
	}

	uint8_t size() const
	{
		return dist(m_wptr, m_rptr);
	}

	value_type operator[](uint8_t i) const
	{
		return m_buffer[next(m_rptr, i)];
	}

	template <typename Writer>
	void copy_to(Writer & writer, uint8_t len) const
	{
		uint8_t rptr = m_rptr;

		if (next(rptr, len) < rptr)
		{
			writer.write(m_buffer + rptr, capacity - rptr);
			writer.write(m_buffer, len - (capacity - rptr));
		}
		else
		{
			writer.write(m_buffer + rptr, len);
		}
	}

	template <typename Reader>
	uint8_t append(Reader & reader, uint8_t len)
	{
		uint8_t wptr = m_wptr;

		uint8_t free = capacity - this->size();
		if (free < len)
			len = free;

		if (wptr > next(wptr, len))
		{
			reader.read(m_buffer + wptr, capacity - wptr);
			reader.read(m_buffer, len - (capacity - wptr));
		}
		else
		{
			reader.read(m_buffer + wptr, len);
		}

		m_wptr = next(wptr, len);
		return len;
	}

	void pop()
	{
		m_rptr = next(m_rptr);
	}

	void pop(uint8_t len)
	{
		m_rptr = next(m_rptr, len);
	}

private:
	volatile value_type m_buffer[capacity];
	volatile uint8_t m_wptr;
	volatile uint8_t m_rptr;

	static uint8_t next(uint8_t v)
	{
		uint8_t res = v + 1;
		if (res == capacity)
			res = 0;
		return res;
	}

	static uint8_t next(uint8_t v, uint8_t len)
	{
		return uint8_t(capacity - v) <= len? len - uint8_t(capacity - v): v + len;
	}

	static uint8_t dist(uint8_t ptr, uint8_t base)
	{
		return ptr >= base? ptr - base: ptr + uint8_t(capacity - base);
	}
};

}

#endif
