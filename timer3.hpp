#ifndef AVRLIB_TIMER3_HPP
#define AVRLIB_TIMER3_HPP

#include <avr/io.h>
#include "timer_base.hpp"

namespace avrlib {

struct timer3
{
	typedef uint16_t value_type;
	static const uint8_t value_bits = 16;

	static value_type value()
	{
		value_type res = TCNT3L;
		res |= TCNT3H << 8;
		return res;
	}

	static void value(value_type v)
	{
		TCNT3H = v >> 8;
		TCNT3L = v;
	}

	static void clock_source(timer_clock_source v)
	{
		TCCR3B = (TCCR3B & 0xf8) | v;
	}

	static void mode(timer_mode v)
	{
		TCCR3A = (TCCR3A & ~0x03) | (v & 0x03);
		TCCR3B = (TCCR3B & ~0x18) | ((v & 0x0c) << 1);
	}

	struct ocra
	{
		typedef uint16_t value_type;
		static void mode(timer_ocr_mode v) { TCCR3A = (TCCR3A & 0x3f) | (v << 6); }
		static void value(value_type v) { OCR3A = v; }
		static value_type value() { return OCR3A; }
		static void interrupt(bool enable)
		{
			if (enable)
			{
				TIFR3 = (1<<OCF3A);
				TIMSK3 |= (1<<OCIE3A);
			}
			else
				TIMSK3 &= (1<<OCIE3A);
		}
	};

	struct ocrb
	{
		typedef uint16_t value_type;
		static void mode(timer_ocr_mode v) { TCCR3A = (TCCR3A & 0xcf) | (v << 4); }
		static void value(value_type v) { OCR3B = v; }
		static value_type value() { return OCR3B; }
		static void interrupt(bool enable)
		{
			if (enable)
			{
				TIFR3 = (1<<OCF3B);
				TIMSK3 |= (1<<OCIE3B);
			}
			else
				TIMSK3 &= (1<<OCIE3B);
		}
	};

	struct ocrc
	{
		typedef uint16_t value_type;
		static void mode(timer_ocr_mode v) { TCCR3A = (TCCR3A & 0xf3) | (v << 2); }
		static void value(value_type v) { OCR3C = v; }
		static value_type value() { return OCR3C; }
		static void interrupt(bool enable)
		{
			if (enable)
			{
				TIFR3 = (1<<OCF3C);
				TIMSK3 |= (1<<OCIE3C);
			}
			else
				TIMSK3 &= (1<<OCIE3C);
		}
	};

	static uint8_t tov_interrupt(bool v)
	{
		if (v)
		{
			TIFR3 = (1<<TOV3);
			TIMSK3 |= (1<<TOIE3);
		}
		else
			TIMSK3 &= ~(1<<TOIE3);
	}

	static bool overflow()
	{
		return TIFR3 & (1<<TOV3);
	}

	static void clear_overflow()
	{
		TIFR3 = (1<<TOV3);
	}
};

}

#endif
