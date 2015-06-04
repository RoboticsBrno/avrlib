#ifndef AVRLIB_BUFFER_HPP
#define AVRLIB_BUFFER_HPP

#include "numeric.hpp"
#include "assert.hpp"

namespace avrlib {

namespace detail {

template <typename T, uint_max_t Capacity, typename Parent>
class buffer_base
	: private Parent
{
public:
	typedef typename least_uint<Capacity + 1>::type index_type;
	typedef T value_type;
	static const index_type capacity = Capacity;

	buffer_base()
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
		return this->next(m_wptr) == m_rptr;
	}

	bool push(value_type v)
	{
		index_type wptr = m_wptr;
		m_buffer[wptr] = v;
		m_wptr = this->next(wptr);
		return m_wptr != m_rptr;
	}

	value_type top() const
	{
		return m_buffer[m_rptr];
	}
	
	volatile value_type const & top_ref() const
	{
		return m_buffer[m_rptr];
	}
	
	volatile value_type & top_ref()
	{
		return m_buffer[m_rptr];
	}

	index_type size() const
	{
		return this->dist(m_wptr, m_rptr);
	}

	value_type operator[](index_type i) const
	{
		return m_buffer[this->next(m_rptr, i)];
	}

	template <typename Writer>
	void copy_to(Writer & writer, index_type len, index_type offset = 0) const
	{
		AVRLIB_ASSERT(len + offset <= this->size());
		uint8_t rptr = this->next(m_rptr, offset);

		if (this->next(rptr, len) < rptr)
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

		if (wptr > this->next(wptr, len))
		{
			reader.read(m_buffer + wptr, capacity - wptr);
			reader.read(m_buffer, len - (capacity - wptr));
		}
		else
		{
			reader.read(m_buffer + wptr, len);
		}

		m_wptr = this->next(wptr, len);
		return len;
	}

	void pop()
	{
		m_rptr = this->next(m_rptr);
	}

	void pop(index_type len)
	{
		m_rptr = this->next(m_rptr, len);
	}

	bool try_pop(value_type & v)
	{
		index_type rptr = m_rptr;
		if (m_wptr == rptr)
			return false;
		v = m_buffer[rptr];
		m_rptr = this->next(rptr);
		return true;
	}

private:
	volatile value_type m_buffer[capacity];
	volatile index_type m_wptr;
	volatile index_type m_rptr;
};

template <uint_max_t Capacity, bool roundCapacity>
class buffer_impl
{
protected:
	typedef typename least_uint<Capacity + 1>::type index_type;

	static index_type next(index_type v)
	{
		index_type res = v + 1;
		if (res == Capacity)
			res = 0;
		return res;
	}

	static index_type next(index_type v, index_type len)
	{
		return index_type(Capacity - v) <= len? len - index_type(Capacity - v): v + len;
	}

	static index_type dist(index_type ptr, index_type base)
	{
		return ptr >= base? ptr - base: ptr + index_type(Capacity - base);
	}
};

template <uint_max_t Capacity>
class buffer_impl<Capacity, true>
{
protected:
	typedef typename least_uint<Capacity + 1>::type index_type;

	static index_type next(index_type v)
	{
		return index_type(v + 1) % Capacity;
	}

	static index_type next(index_type v, index_type len)
	{
		return index_type(v + len) % Capacity;
	}

	static index_type dist(index_type ptr, index_type base)
	{
		return index_type(ptr - base) % Capacity;
	}
};

}

template <typename T, uint_max_t Capacity>
class buffer
	: public detail::buffer_base<T, Capacity, detail::buffer_impl<Capacity, (Capacity & (Capacity-1)) == 0> >
{	
};

}

#endif
