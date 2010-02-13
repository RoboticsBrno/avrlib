#ifndef AVRLIB_FIXEDPOINT_HPP
#define AVRLIB_FIXEDPOINT_HPP

namespace avrlib {

template <typename T, int exp>
class fixedpoint
{
public:
	typedef T value_type;

	fixedpoint()
	{
	}

	template <typename U>
	fixedpoint(U value)
	{
		this->set(value);
	}

	template <typename U>
	fixedpoint(U value, int valexp)
	{
		this->set(value,  valexp);
	}

	template <typename U, int uexp>
	fixedpoint(fixedpoint<U, uexp> const & u)
	{
		this->set(u.get_raw(),  uexp);
	}

	value_type get() const
	{
		return m_value / (1<<exp);
	}

	value_type get_raw() const
	{
		return m_value;
	}

	void set(value_type value)
	{
		m_value = value * (1<<exp);
	}

	void set(value_type value, int valexp)
	{
		int resexp = exp - valexp;
		m_value = value * (1<<resexp);
	}

	fixedpoint & operator+=(fixedpoint const & rhs)
	{
		m_value += rhs.m_value;
		return *this;
	}

	friend bool operator==(fixedpoint const & lhs, fixedpoint const & rhs)
	{
		return lhs.m_value == rhs.m_value;
	}

	friend bool operator!=(fixedpoint const & lhs, fixedpoint const & rhs)
	{
		return lhs.m_value != rhs.m_value;
	}

	friend bool operator<(fixedpoint const & lhs, fixedpoint const & rhs)
	{
		return lhs.m_value < rhs.m_value;
	}

	friend bool operator>(fixedpoint const & lhs, fixedpoint const & rhs)
	{
		return lhs.m_value > rhs.m_value;
	}

	friend bool operator<=(fixedpoint const & lhs, fixedpoint const & rhs)
	{
		return lhs.m_value <= rhs.m_value;
	}

	friend bool operator>=(fixedpoint const & lhs, fixedpoint const & rhs)
	{
		return lhs.m_value >= rhs.m_value;
	}

private:
	T m_value;
};

template <int exp, typename T>
fixedpoint<T, exp> make_fixedpoint(T value, int valexp = exp)
{
	return fixedpoint<T, exp>(value, valexp);
}

}

#endif
