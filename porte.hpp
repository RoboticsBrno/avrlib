#ifndef AVRLIB_PORTE_HPP
#define AVRLIB_PORTE_HPP

#include <avr/io.h>

namespace avrlib {

struct porte
{
	static uint8_t port() { return PORTE; }
	static void port(uint8_t v) { PORTE = v; }

	static uint8_t pin() { return PINE; }
	static void pin(uint8_t v) { PINE = v; }

	static uint8_t dir() { return DDRE; }
	static void dir(uint8_t v) { DDRE = v; }
};

}

#endif
