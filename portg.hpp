#ifndef AVRLIB_PORTG_HPP
#define AVRLIB_PORTG_HPP

#include <avr/io.h>

namespace avrlib {

struct portg
{
	static uint8_t port() { return PORTG; }
	static void port(uint8_t v) { PORTG = v; }

	static uint8_t pin() { return PING; }
	static void pin(uint8_t v) { PING = v; }

	static uint8_t dir() { return DDRG; }
	static void dir(uint8_t v) { DDRG = v; }
};

}

#endif
