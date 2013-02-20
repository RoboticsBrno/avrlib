#ifndef AVRLIB_PORTD_HPP
#define AVRLIB_PORTD_HPP

#include <avr/io.h>

namespace avrlib {

struct portd
{
	static uint8_t port() { return PORTD; }
	static void port(uint8_t v) { PORTD = v; }

	static uint8_t pin() { return PIND; }
	static void pin(uint8_t v) { PIND = v; }

	static uint8_t dir() { return DDRD; }
	static void dir(uint8_t v) { DDRD = v; }
};

}

#endif
