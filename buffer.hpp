#ifndef AVRLIB_BUFFER_HPP
#define AVRLIB_BUFFER_HPP

#include "numeric.hpp"

namespace avrlib {

template <typename T, uint_max_t Capacity>
class buffer
{
public:
	typedef typename least_uint<Capacity + 1>::type index_type;
	typedef T value_type;
	static const index_type capacity = Capacity;

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

	index_type size() const
	{
		return dist(m_wptr, m_rptr);
	}

	value_type operator[](index_type i) const
	{
		return m_buffer[next(m_rptr, i)];
	}

	template <typename Writer>
	void copy_to(Writer & writer, index_type len) const
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
	index_type append(Reader & reader, index_type len)
	{
		index_type wptr = m_wptr;

		index_type free = capacity - this->size();
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

	void pop(index_type len)
	{
		m_rptr = next(m_rptr, len);
	}

private:
	volatile value_type m_buffer[capacity];
	volatile index_type m_wptr;
	volatile index_type m_rptr;

	static index_type next(index_type v)
	{
		index_type res = v + 1;
		if (res == capacity)
			res = 0;
		return res;
	}

	static index_type next(index_type v, index_type len)
	{
		return index_type(capacity - v) <= len? len - index_type(capacity - v): v + len;
	}

	static index_type dist(index_type ptr, index_type base)
	{
		return ptr >= base? ptr - base: ptr + index_type(capacity - base);
	}
};

}

#endif
