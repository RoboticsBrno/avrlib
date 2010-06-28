#ifndef AVRLIB_ATOMIC_HPP
#define AVRLIB_ATOMIC_HPP

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
