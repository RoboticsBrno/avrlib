#ifndef AVRLIB_PORTB_HPP
#define AVRLIB_PORTB_HPP

#include <avr/io.h>

namespace avrlib {

struct portb
{
	static uint8_t port() { return PORTB; }
	static void port(uint8_t v) { PORTB = v; }

	static uint8_t pin() { return PINB; }
	static void pin(uint8_t v) { PINB = v; }

	static uint8_t dir() { return DDRB; }
	static void dir(uint8_t v) { DDRB = v; }
};

}

#endif
