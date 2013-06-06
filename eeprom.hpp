#ifndef AVRLIB_EEPROM_HPP
#define AVRLIB_EEPROM_HPP

#include <avr/io.h>

namespace avrlib {

inline void load_eeprom(uint8_t address, uint8_t * ptr, uint8_t len)
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
T load_eeprom(uint8_t address, T& data)
{
	T res;
	load_eeprom(address, (uint8_t *)&res, sizeof res);
	data = res;
	return res;
}

inline void store_eeprom(uint8_t address, uint8_t const * ptr, uint8_t len)
{
#ifndef EEMWE
# define EEMWE_ EEMPE
# define EEWE_ EEPE
#else
# define EEMWE_ EEMWE
# define EEWE_ EEWE
#endif

	uint8_t const * pend = ptr + len;

	EEARH = 0;
	EEARL = address;

	while (ptr != pend)
	{
		EEDR = *ptr++;

		EECR = (1<<EEMWE_);
		EECR = (1<<EEWE_);

		while (EECR & (1<<EEWE_))
		{
		}

		++EEARL;
	}

#undef EEMWE_
#undef EEWE_
}

template <typename T>
void store_eeprom(uint8_t address, T value)
{
	store_eeprom(address, (uint8_t const *)&value, sizeof value);
}

}

#endif
