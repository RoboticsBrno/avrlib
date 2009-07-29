#ifndef AVRLIB_EEPROM_HPP
#define AVRLIB_EEPROM_HPP

#include <avr/io.h>

namespace avrlib {

void load_eeprom(uint8_t address, uint8_t * ptr, uint8_t len)
{
	uint8_t * pend = ptr + len;
	
	EEARH = 0;
	EEARL = address;

	while (ptr != pend)
	{
		EECR = (1<<EERE);
		*ptr++ = EEDR;
		++EEARL;
	}
}

template <typename T>
T load_eeprom(uint8_t address)
{
	T res;
	load_eeprom(address, (uint8_t const *)&res, sizeof res);
	return res;
}

void store_eeprom(uint8_t address, uint8_t const * ptr, uint8_t len)
{
	uint8_t const * pend = ptr + len;
	
	EEARH = 0;
	EEARL = address;

	while (ptr != pend)
	{
		EEDR = *ptr++;
		
		EECR = (1<<EEMWE);
		EECR = (1<<EEWE);
		
		while (EECR & (1<<EEWE))
		{
		}

		++EEARL;
	}
}

template <typename T>
void store_eeprom(uint8_t address, T value)
{
	store_eeprom(address, (uint8_t const *)&value, sizeof value);
}

}

#endif
