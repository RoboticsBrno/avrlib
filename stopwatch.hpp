#ifndef AVRLIB_STOPWATCH_HPP
#define AVRLIB_STOPWATCH_HPP

namespace avrlib {

template <typename Timer>
void wait(Timer & timer, typename Timer::time_type time)
{
	typename Timer::time_type base = timer.value();
	while (time > 0)
	{
		typename Timer::time_type new_base = timer.value();
		typename Timer::time_type difference = new_base - base;
		if (time < difference)
			break;
		time -= difference;
		base = new_base;
	}
}

template <typename Timer, typename Process>
void wait(Timer & timer, typename Timer::time_type time, Process process)
{
	typename Timer::time_type base = timer.value();
	while (time > 0)
	{
		typename Timer::time_type new_base = timer.value();
		typename Timer::time_type difference = new_base - base;
		if (time < difference)
			break;
		time -= difference;
		base = new_base;
		process();
	}
}

template <typename Timer, typename Process>
typename Timer::time_type wait(Timer & timer, typename Timer::time_type time, Process process, int)
{
	typename Timer::time_type base = timer.value();
	if(time == 0)
		for(;;)
			if(process())
				return timer.value() - base;
	typename Timer::time_type base_time = base;
	while (time > 0)
	{
		typename Timer::time_type new_base = timer.value();
		typename Timer::time_type difference = new_base - base;
		if (time < difference)
			return base_time - time;
		time -= difference;
		base = new_base;
		if(process())
			return base_time - time;
	}
	return timer.value() - base_time;
}

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
			m_base = m_timer.value();
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
		m_base = m_timer.value();
	}

	void start()
	{
		if (!m_running)
		{
			m_base = m_timer.value() - m_base;
			m_running = true;
		}
	}

	void stop()
	{
		if (m_running)
		{
			m_base = m_timer.value() - m_base;
			m_running = false;
		}
	}

	time_type operator()() const
	{
		if (m_running)
			return m_timer.value() - m_base;
		else
			return m_base;
	}
	
	void set(time_type value)
	{
		if (m_running)
			m_base = m_timer.value() - value;
		else
			m_base = value;
	}

	void decrease(time_type time)
	{
		if (m_running)
			m_base += time;
		else
			m_base -= time;
	}

private:
	Timer const & m_timer;
	volatile bool m_running;
	volatile time_type m_base;
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

	void force()
	{
		this->set(m_timeout + 1);
	}

	void ack()
	{
		this->decrease(m_timeout);
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
