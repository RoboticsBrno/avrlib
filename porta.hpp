#ifndef AVRLIB_PORTA_HPP
#define AVRLIB_PORTA_HPP

#include <avr/io.h>

namespace avrlib {

struct porta
{
	static uint8_t port() { return PORTA; }
	static void port(uint8_t v) { PORTA = v; }

	static uint8_t pin() { return PINA; }
	static void pin(uint8_t v) { PINA = v; }

	static uint8_t dir() { return DDRA; }
	static void dir(uint8_t v) { DDRA = v; }
};

}

#endif
