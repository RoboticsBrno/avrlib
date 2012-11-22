#ifndef AVRLIB_TIMER3_HPP
#define AVRLIB_TIMER3_HPP

#include <avr/io.h>
#include "timer_base.hpp"

namespace avrlib {
	
#ifndef TIFR3
#define TIFR3 ETIFR
#endif
#ifndef TIMSK3
#define TIMSK3 ETIMSK
#endif

#ifndef ICIE3
# define ICIE3 TICIE3
#endif

struct timer3
{
	typedef timer_16b_value_type value_type;
	typedef value_type time_type;
	static volatile uint8_t value_bits;

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
		if((v & (1<<3)) != 0)
		{
			if((v & (1<<0)) == 0)
				top_value = (volatile value_type*)(&ICR3);
			else
				top_value = (volatile value_type*)(&OCR3A);
		}
		else if (v == 4)
		{
			top_value = (volatile value_type*)(&OCR3A);
		}			
		else
		{
			switch (v&3)
			{
			case 0:
				top_value = (volatile value_type*)(&timer_top_16b);
				value_bits = 16;
			break;
			case 1:
				top_value = (volatile value_type*)(&timer_top_8b);
				value_bits = 8;
				break;
			case 2:
				top_value = (volatile value_type*)(&timer_top_9b);
				value_bits = 9;
				break;
			case 3:
				top_value = (volatile value_type*)(&timer_top_10b);
				value_bits = 10;
				break;
			}
		}
	}
	
	static value_type top()
	{
		return *top_value;
	}

	struct ocra
	{
		typedef uint16_t value_type;
		static void mode(timer_ocr_mode v) { TCCR3A = (TCCR3A & 0x3f) | (v << 6); }
		static timer_ocr_mode mode() { return (TCCR3A & ~0x3f) >> 6; }
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
		static timer_ocr_mode mode() { return (TCCR3A & ~0xcf) >> 6; }
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

#ifdef OCR3C
	struct ocrc
	{
		typedef uint16_t value_type;
		static void mode(timer_ocr_mode v) { TCCR3A = (TCCR3A & 0xf3) | (v << 2); }
		static timer_ocr_mode mode() { return (TCCR3A & ~0xf3) >> 6; }
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
#endif

	struct icr
	{
		static void value(value_type v) { ICR3 = v; }
		static value_type value() { return ICR3; }

		static void rising_edge(bool enable)
		{
			TCCR3B = (TCCR3B & ~(1<<ICES3)) | (enable? (1<<ICES3): 0);
		}

		static void noise_canceler(bool enable)
		{
			TCCR3B = (TCCR3B & ~(1<<ICNC3)) | (enable? (1<<ICNC3): 0);
		}

		static void interrupt(bool enable)
		{
			if (enable)
			{
				TIFR3 = (1<<ICF3);
				TIMSK3 |= (1<<ICIE3);
			}
			else
				TIMSK3 &= (1<<ICIE3);
		}

		static bool captured()
		{
			return TIFR3 & (1<<ICF3);
		}

		static void clear_captured()
		{
			TIFR3 = (1<<ICF3);
		}
	};

	static void tov_interrupt(bool v)
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
	
private:
	static volatile value_type* top_value;
};

volatile uint8_t timer3::value_bits = 16;
volatile timer3::value_type* timer3::top_value = (volatile timer3::value_type*)(&timer_top_16b);

}

#endif
