#ifndef AVRLIB_TIMER_XMEGA_HPP
#define AVRLIB_TIMER_XMEGA_HPP

#include <avr/io.h>
#include "timer_base.hpp"

#define AVRLIB_XMEGA_TIMER(timer_name, timer) \
	struct timer_name \
	{ \
		typedef uint16_t value_type; \
		typedef value_type time_type; \
		static inline value_type value() { return timer##_CNT; } \
		static inline void value(const value_type& v) { timer##_CNT = v; } \
		static inline value_type top() { return timer##_PER; } \
		static void top(const value_type& v) \
		{ \
			timer##_PER = v; \
			switch (v) \
			{ \
				case     0: value_bits = 16; break; \
				case     1: value_bits =  0; break; \
				case     2: value_bits =  1; break; \
				case     4: value_bits =  2; break; \
				case     8: value_bits =  3; break; \
				case    16: value_bits =  4; break; \
				case    32: value_bits =  5; break; \
				case    64: value_bits =  6; break; \
				case   128: value_bits =  7; break; \
				case   256: value_bits =  8; break; \
				case   512: value_bits =  9; break; \
				case  1024: value_bits = 10; break; \
				case  2048: value_bits = 11; break; \
				case  4096: value_bits = 12; break; \
				case  8192: value_bits = 13; break; \
				case 16384: value_bits = 14; break; \
				case 32768: value_bits = 15; break; \
			} \
		} \
		static inline void clock_source(const avrlib::timer_clock_source& prescaler) { timer##_CTRLA = prescaler; } \
		static inline uint8_t overflow() { return timer##_INTFLAGS & TC0_OVFIF_bm; } \
		static inline void clear_overflow() { timer##_INTFLAGS = TC0_OVFIF_bm; } \
		static void tov_interrupt(uint8_t v) \
		{ \
			if(v) \
			{ \
				timer##_INTFLAGS = TC0_OVFIF_bm; \
				timer##_INTCTRLA = (timer##_INTCTRLA & ~TC0_OVFINTLVL_gm) | ovf_int_lvl; \
			} \
			else \
				timer##_INTCTRLA = (timer##_INTCTRLA & ~TC0_OVFINTLVL_gm); \
		} \
		static void tov_level(const uint8_t& lvl) \
		{ \
			ovf_int_lvl = (lvl & TC0_OVFINTLVL_gm); \
			timer##_INTCTRLA = (timer##_INTCTRLA & ~TC0_OVFINTLVL_gm) | ovf_int_lvl; \
		} \
		static volatile uint8_t value_bits; \
	private: \
		static volatile uint8_t ovf_int_lvl; \
	}; \
	volatile uint8_t timer_name::value_bits = 16; \
	volatile uint8_t timer_name::ovf_int_lvl = 2
	
#endif // AVRLIB_TIMER_XMEGA