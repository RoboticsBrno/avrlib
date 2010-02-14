#ifndef AVRLIB_TIMER_HPP
#define AVRLIB_TIMER_HPP

#include <stdint.h>
#include "timer_base.hpp"

namespace avrlib {

template <typename Timer, typename Time = uint32_t>
struct counter
{
	typedef Timer timer_type;
	typedef Time time_type;

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

	time_type operator()() const
	{
		cli();
		for (;;)
		{
			uint16_t overflows = static_cast<uint16_t volatile const &>(m_overflows);
			typename timer_type::value_type postval = timer_type::value();

			if (!timer_type::overflow())
			{
				sei();
				return ((time_type)overflows << timer_type::value_bits) | postval;
			}

			tov_interrupt();
			timer_type::clear_overflow();
		}
	}

	void tov_interrupt() const
	{
		++static_cast<uint16_t volatile &>(m_overflows);
	}

private:
	mutable uint16_t m_overflows;
};

}

#endif
