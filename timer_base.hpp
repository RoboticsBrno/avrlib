#ifndef AVRLIB_TIMER_BASE_HPP
#define AVRLIB_TIMER_BASE_HPP

namespace avrlib {

enum timer_clock_source
{
	timer_no_clock = 0,
	timer_fosc_1 = 1,
	timer_fosc_8 = 2,
	timer_fosc_64 = 3,
	timer_fosc_256 = 4,
	timer_fosc_1024 = 5,
	timer_ext_falling = 6,
	timer_ext_rising = 7,
};

enum timer_mode
{
	timer_mode_normal = 0,
	timer_mode_ctc = 4,
};

enum timer_ocr_mode
{
	timer_ocr_mode_disconnected = 0,
	timer_ocr_mode_toggle = 1,
	timer_ocr_mode_positive = 2,
	timer_ocr_mode_negative = 3,
};

}

#endif
