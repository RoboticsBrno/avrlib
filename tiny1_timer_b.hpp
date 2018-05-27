#ifndef AVRLIB_TIYN1_TIMER_B_HPP
#define AVRLIB_TIYN1_TIMER_B_HPP

#include <avr/io.h>
#include "timer_base.hpp"

namespace avrlib
{

class timer_b_t
{
public:
	typedef uint16_t value_type;
	typedef value_type time_type;

	timer_b_t(TCB_t& timer)
		:value_bits(16), m_timer(timer)
	{}

	value_type value() { return m_timer.CNT; }
	void value(const value_type& v) { m_timer.CNT = v; }
	value_type top() { return m_timer.CCMP; }
	void top(const value_type& v)
	{
		m_timer.CCMP = v;
		switch (v)
		{
			case 65535:
			case     0: value_bits = 16; break;
			case     1: value_bits =  0; break;
			case     2: value_bits =  1; break;
			case     4: value_bits =  2; break;
			case     8: value_bits =  3; break;
			case    16: value_bits =  4; break;
			case    32: value_bits =  5; break;
			case    64: value_bits =  6; break;
			case   128: value_bits =  7; break;
			case   256: value_bits =  8; break;
			case   512: value_bits =  9; break;
			case  1024: value_bits = 10; break;
			case  2048: value_bits = 11; break;
			case  4096: value_bits = 12; break;
			case  8192: value_bits = 13; break;
			case 16384: value_bits = 14; break;
			case 32768: value_bits = 15; break;
		}
	}

	void clock_source(const avrlib::timer_clock_source& prescaler)
	{
		if (prescaler == timer_no_clock)
			m_timer.CTRLA &= ~TCB_ENABLE_bm;
		else
			m_timer.CTRLA = (((prescaler - 1)<<TCB_CLKSEL_gp) & TCB_CLKSEL_gm) | TCB_ENABLE_bm;
	}
	uint8_t overflow() { return m_timer.INTFLAGS & TCB_CAPT_bm; }
	void clear_overflow() { m_timer.INTFLAGS = TCB_CAPT_bm; }
	void tov_interrupt(uint8_t v)
	{
		if(v)
		{
			m_timer.INTFLAGS = TCB_CAPT_bm;
			m_timer.INTCTRL |= TCB_CAPT_bm;
		}
		else
			m_timer.INTCTRL &= ~TCB_CAPT_bm;
	}

	volatile uint8_t value_bits;

private:
	TCB_t& m_timer;
};

} // namespace avrlib

	
#endif // AVRLIB_TIYN1_TIMER_B
