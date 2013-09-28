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

}

#endif
