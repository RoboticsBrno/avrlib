#ifndef AVRLIB_TIMER_HPP
#define AVRLIB_TIMER_HPP

#include <stdint.h>
#include "timer_base.hpp"

namespace avrlib {

template <typename Timer, typename Time = uint32_t, typename Overflow = uint16_t, bool full_bit_period = true>
struct counter
{
	typedef Timer timer_type;
	typedef Time time_type;
	typedef Overflow overflow_type;

	template <time_type v>
	struct us : timer_type::template us<v>
	{
	};

	counter()
		: m_overflows(0)
	{
		timer_type::value(0);
	}

	template <typename Prescaler>
	explicit counter(Prescaler const & prescaler)
		: m_overflows(0)
	{
		timer_type::value(0);
		this->enable(prescaler);
	}

	~counter()
	{
		this->disable();
	}

	void enable(timer_clock_source prescaler)
	{
		timer_type::clock_source(prescaler);
		timer_type::tov_interrupt(true);
	}

	void disable()
	{
		timer_type::tov_interrupt(false);
		timer_type::clock_source(timer_no_clock);
	}

	time_type value() const
	{
		typename timer_type::time_type time = timer_type::value();
		overflow_type overflows;

		for (;;)
		{
			// This is needed for correctness in the case of interrupt congestion.
			// Perhaps we could afford to cli, process and sei, but that would screw up
			// interrupt priorities.
			while (timer_type::overflow()) {}

			overflows = static_cast<overflow_type volatile const &>(m_overflows);
			typename timer_type::time_type new_time = timer_type::value();

			if (new_time >= time)
			{
				if(full_bit_period)
					return (time_type(overflows) << timer_type::value_bits) | new_time;
				else
					return (time_type(overflows) * timer_type::top()) + new_time;
			}				

			time = new_time;
		}
	}
	
	inline time_type operator()() const { return value(); }

	void tov_interrupt() const
	{
		++static_cast<overflow_type volatile &>(m_overflows);
	}

private:
	mutable overflow_type m_overflows;
};


template <typename Timer, typename Time = uint32_t, typename Overflow = uint16_t, bool full_bit_period = true>
struct counter_ns
{
	typedef Timer timer_type;
	typedef Time time_type;
	typedef Overflow overflow_type;

	template <time_type v>
	struct us : timer_type::template us<v>
	{
	};

	counter_ns(timer_type& timer)
		: m_timer(timer), m_overflows(0)
	{
		m_timer.value(0);
	}

	template <typename Prescaler>
	explicit counter_ns(timer_type& timer, Prescaler const & prescaler)
		: m_timer(timer), m_overflows(0)
	{
		m_timer.value(0);
		this->enable(prescaler);
	}

	~counter_ns()
	{
		this->disable();
	}

	void enable(timer_clock_source prescaler)
	{
		m_timer.clock_source(prescaler);
		m_timer.tov_interrupt(true);
	}

	void disable()
	{
		m_timer.tov_interrupt(false);
		m_timer.clock_source(timer_no_clock);
	}

	time_type value() const
	{
		typename timer_type::time_type time = m_timer.value();
		overflow_type overflows;

		for (;;)
		{
			// This is needed for correctness in the case of interrupt congestion.
			// Perhaps we could afford to cli, process and sei, but that would screw up
			// interrupt priorities.
			while (m_timer.overflow()) {}

			overflows = static_cast<overflow_type volatile const &>(m_overflows);
			typename timer_type::time_type new_time = m_timer.value();

			if (new_time >= time)
			{
				if(full_bit_period)
					return (time_type(overflows) << m_timer.value_bits) | new_time;
				else
					return (time_type(overflows) * m_timer.top()) + new_time;
			}

			time = new_time;
		}
	}
	
	inline time_type operator()() const { return value(); }

	void tov_interrupt() const
	{
		++static_cast<overflow_type volatile &>(m_overflows);
	}

private:
	timer_type& m_timer;
	mutable overflow_type m_overflows;
};

}

#endif
