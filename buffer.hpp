#ifndef AVRLIB_BUFFER_HPP
#define AVRLIB_BUFFER_HPP

#include <stdint.h>

namespace avrlib {

template <typename T, uint8_t size>
class buffer
{
public:
	typedef T value_type;

	buffer()
		: m_wptr(0), m_rptr(0)
	{
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

	void pop()
	{
		m_rptr = next(m_rptr);
	}

private:
	value_type m_buffer[size];
	uint8_t m_wptr;
	uint8_t m_rptr;

	static uint8_t next(uint8_t v)
	{
		uint8_t res = v + 1;
		if (res == size)
			res = 0;
		return res;
	}
};

}

#endif
