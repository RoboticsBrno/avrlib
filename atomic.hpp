#ifndef AVRLIB_ATOMIC_HPP
#define AVRLIB_ATOMIC_HPP

// Atomic `*ptr &= ~mask`
#define avrlib_atomic_clear(ptr, mask) \
    asm volatile ("lac Z, %1":: "z"(ptr), "r"((uint8_t)(mask)): "memory")

// Atomic `*ptr |= mask`
#define avrlib_atomic_set(ptr, mask) \
    asm volatile ("las Z, %1":: "z"(ptr), "r"((uint8_t)(mask)): "memory")

// Atomic `*ptr ^= mask`
#define avrlib_atomic_toggle(ptr, mask) \
    asm volatile ("lat Z, %1":: "z"(ptr), "r"((uint8_t)(mask)): "memory")

namespace avrlib {

template <typename T>
class atomic
{
public:
	typedef T value_type;

	atomic()
	{
	}

	explicit atomic(value_type value)
		: m_value(value)
	{
	}

	value_type load() const volatile
	{
		return m_value;
	}

	value_type load_consume() const
	{
		return m_value;
	}

	operator value_type() const volatile
	{
		return m_value;
	}

	void store(value_type value) volatile
	{
		m_value = value;
	}

	value_type operator=(value_type value) volatile
	{
		m_value = value;
	}

private:
	value_type m_value;

	atomic(atomic const &);
	atomic & operator=(atomic const &);
};

}

#endif
