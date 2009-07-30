#ifndef AVRLIB_STOPWATCH_HPP
#define AVRLIB_STOPWATCH_HPP

namespace avrlib {

template <typename Timer>
class stopwatch
{
public:
	typedef typename Timer::time_type time_type;

	explicit stopwatch(Timer const & timer)
		: m_timer(timer)
	{
		this->clear();
	}

	void clear()
	{
		m_base = m_timer();
	}

	time_type operator()() const
	{
		return m_timer() - m_base;
	}

private:
	Timer const & m_timer;
	time_type m_base;
};

template <typename Timer>
class timeout
	: public stopwatch<Timer>
{
public:
	typedef stopwatch<Timer> base_type;
	typedef typename base_type::time_type time_type;

	timeout(Timer const & timer, time_type timeout)
		: stopwatch<Timer>(timer), m_timeout(timeout)
	{
	}

	operator bool() const
	{
		return (*this)() > m_timeout;
	}

private:
	time_type m_timeout;
};

}

#endif
