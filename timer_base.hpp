#ifndef AVRLIB_TIMER_BASE_HPP
#define AVRLIB_TIMER_BASE_HPP

namespace avrlib {

#if __AVR_ARCH__ >= 100 /*xmega*/

enum timer_clock_source
{
	timer_no_clock = 0,
	timer_fosc_1 = 1,
	timer_fosc_2 = 2,
	timer_fosc_4 = 3,
	timer_fosc_8 = 4,
	timer_fosc_64 = 5,
	timer_fosc_256 = 6,
	timer_fosc_1024 = 7,
	timer_event_0 = 8,
	timer_event_1 = 9,
	timer_event_2 = 10,
	timer_event_3 = 11,
	timer_event_4 = 12,
	timer_event_5 = 13,
	timer_event_6 = 14,
	timer_event_7 = 15
};

#else /*mega*/

enum timer_clock_source
{
	timer_no_clock = 0,
	timer_fosc_1 = 1,
	timer_fosc_8 = 2,
	timer_fosc_64 = 3,
	timer_fosc_256 = 4,
	timer_fosc_1024 = 5,
	timer_ext_falling = 6,
	timer_ext_rising = 7
};

static const uint16_t timer_clock_prescaler_values [] = 
{
	0, 1, 8, 64, 256, 1024, 0xffff, 0xfffe
};

enum timer_mode
{
	timer_mode_normal,
	timer_mode_pwmp_8,
	timer_mode_pwmp_9,
	timer_mode_pwmp_10,
	timer_mode_ctc_ocra,
	timer_mode_pwm_8,
	timer_mode_pwm_9,
	timer_mode_pwm_10,
	timer_mode_pwmpf_icr,
	timer_mode_pwmpf_ocra,
	timer_mode_pwmp_icr,
	timer_mode_pwmp_ocra,
	timer_mode_ctc_icr,
	timer_mode_13,
	timer_mode_pwm_icr,
	timer_mode_pwm_ocra
};

enum timer_ocr_mode
{
	timer_ocr_mode_disconnected = 0,
	timer_ocr_mode_toggle = 1,
	timer_ocr_mode_positive = 2,
	timer_ocr_mode_negative = 3
};

typedef uint16_t timer_16b_value_type;

static const timer_16b_value_type timer_top_8b  = 0x00FF;
static const timer_16b_value_type timer_top_9b  = 0x01FF;
static const timer_16b_value_type timer_top_10b = 0x03FF;
static const timer_16b_value_type timer_top_16b = 0xFFFF;

#endif /* mega / xmega */

}

#endif
