#ifndef AVRLIB_TIMER1_HPP
#define AVRLIB_TIMER1_HPP

#include <avr/io.h>
#include "timer_base.hpp"

#ifndef TIFR1
# define TIFR1 TIFR
#endif

#ifndef TIMSK1
# define TIMSK1 TIMSK
#endif

#ifndef ICIE1
# define ICIE1 TICIE1
#endif

namespace avrlib {

struct timer1
{
	typedef timer_16b_value_type value_type;
	typedef value_type time_type;
	static volatile uint8_t value_bits;

	static value_type value()
	{
		value_type res = TCNT1L;
		res |= TCNT1H << 8;
		return res;
	}

	static void value(value_type v)
	{
		TCNT1H = v >> 8;
		TCNT1L = v;
	}

	static void clock_source(timer_clock_source v)
	{
		TCCR1B = (TCCR1B & 0xf8) | v;
	}

	static void mode(timer_mode v)
	{
		TCCR1A = (TCCR1A & ~0x03) | (v & 0x03);
		TCCR1B = (TCCR1B & ~0x18) | ((v & 0x0c) << 1);
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
		static void mode(timer_ocr_mode v) { TCCR1A = (TCCR1A & 0x3f) | (v << 6); }
		static void value(value_type v) { OCR1A = v; }
		static value_type value() { return OCR1A; }
		static void interrupt(bool enable)
		{
			if (enable)
			{
				TIFR1 = (1<<OCF1A);
				TIMSK1 |= (1<<OCIE1A);
			}
			else
				TIMSK1 &= (1<<OCIE1A);
		}
	};

	struct ocrb
	{
		static void mode(timer_ocr_mode v) { TCCR1A = (TCCR1A & 0xcf) | (v << 4); }
		static void value(value_type v) { OCR1B = v; }
		static value_type value() { return OCR1B; }
		static void interrupt(bool enable)
		{
			if (enable)
			{
				TIFR1 = (1<<OCF1B);
				TIMSK1 |= (1<<OCIE1B);
			}
			else
				TIMSK1 &= (1<<OCIE1B);
		}
	};

#ifdef OCR1C
	struct ocrc
	{
		static void mode(timer_ocr_mode v) { TCCR1A = (TCCR1A & 0xf3) | (v << 2); }
		static void value(value_type v) { OCR1C = v; }
		static value_type value() { return OCR1C; }
			
#ifdef __AVR_ATmega128__
		static void interrupt(bool enable)
		{
			if (enable)
			{
				TIFR1 = (1<<OCF1C);
				TIMSK1 |= (1<<OCIE1C);
			}
			else
				TIMSK1 &= (1<<OCIE1C);
		}
#else
		static void interrupt(bool enable)
		{
			if (enable)
			{
				ETIFR = (1<<OCF1C);
				ETIMSK |= (1<<OCIE1C);
			}
			else
				ETIMSK &= (1<<OCIE1C);
		}
#endif
	};
#endif

	struct icr
	{
		static void value(value_type v) { ICR1 = v; }
		static value_type value() { return ICR1; }

		static void rising_edge(bool enable)
		{
			TCCR1B = (TCCR1B & ~(1<<ICES1)) | (enable? (1<<ICES1): 0);
		}

		static void noise_canceler(bool enable)
		{
			TCCR1B = (TCCR1B & ~(1<<ICNC1)) | (enable? (1<<ICNC1): 0);
		}

		static void interrupt(bool enable)
		{
			if (enable)
			{
				TIFR1 = (1<<ICF1);
				TIMSK1 |= (1<<ICIE1);
			}
			else
				TIMSK1 &= (1<<ICIE1);
		}

		static bool captured()
		{
			return TIFR1 & (1<<ICF1);
		}

		static void clear_captured()
		{
			TIFR1 = (1<<ICF1);
		}
	};

	static void tov_interrupt(bool v)
	{
		if (v)
		{
			TIFR1 = (1<<TOV1);
			TIMSK1 |= (1<<TOIE1);
		}
		else
			TIMSK1 &= ~(1<<TOIE1);
	}

	static bool overflow()
	{
		return TIFR1 & (1<<TOV1);
	}

	static void clear_overflow()
	{
		TIFR1 = (1<<TOV1);
	}
	
private:
	static volatile value_type* top_value;
};

volatile uint8_t timer1::value_bits = 16;
volatile timer1::value_type* timer1::top_value = (volatile timer1::value_type*)(&timer_top_16b);

}

#endif
