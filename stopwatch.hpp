#ifndef AVRLIB_STOPWATCH_HPP
#define AVRLIB_STOPWATCH_HPP

namespace avrlib {

template <typename Timer>
class stopwatch
{
public:
	typedef typename Timer::time_type time_type;

	explicit stopwatch(Timer const & timer)
		: m_timer(timer), m_running(true)
	{
		this->clear();
	}

	void clear()
	{
		if (m_running)
			m_base = m_timer();
		else
			m_base = 0;
	}

	void cancel()
	{
		m_running = false;
		m_base = 0;
	}

	void restart()
	{
		m_running = true;
		m_base = m_timer();
	}

	void start()
	{
		if (!m_running)
		{
			m_base = m_timer() - m_base;
			m_running = true;
		}
	}

	void stop()
	{
		if (m_running)
		{
			m_base = m_timer() - m_base;
			m_running = false;
		}
	}

	time_type operator()() const
	{
		if (m_running)
			return m_timer() - m_base;
		else
			return m_base;
	}

private:
	Timer const & m_timer;
	bool m_running;
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
