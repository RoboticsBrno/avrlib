#ifndef AVRLIB_PORTC_HPP
#define AVRLIB_PORTC_HPP

#include <avr/io.h>

namespace avrlib {

struct portc
{
	static uint8_t port() { return PORTC; }
	static void port(uint8_t v) { PORTC = v; }

	static uint8_t pin() { return PINC; }
	static void pin(uint8_t v) { PINC = v; }

	static uint8_t dir() { return DDRC; }
	static void dir(uint8_t v) { DDRC = v; }
};

}

#endif
