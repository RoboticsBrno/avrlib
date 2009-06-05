#ifndef AVRLIB_FORMAT_HPP
#define AVRLIB_FORMAT_HPP

#include <stdint.h>

namespace avrlib {

template <typename Stream>
void send(Stream & s, char const * str)
{
	for (; *str != 0; ++str)
		s.write(*str);
}

template <typename Stream, typename Unsigned>
void send_hex(Stream & s, Unsigned v, uint8_t width = 0, char fill = '0')
{
	static char const digits[] = "0123456789ABCDEF";

	char buf[32];
	uint8_t i = 0;

	if (v == 0)
	{
		buf[i++] = '0';
	}
	else if (v < 0)
	{
		buf[i++] = '*';
	}
	else
	{
		for (; v != 0; v >>= 4)
		{
			buf[i++] = digits[v & 0xF];
		}
	}

	while (i < width)
		buf[i++] = fill;
	
	for (; i > 0; --i)
		s.write(buf[i - 1]);
}

template <typename Stream, typename Integer>
void send_int(Stream & s, Integer v, uint8_t width = 0, char fill = ' ')
{
	char buf[32];
	uint8_t i = 0;
	bool negative = false;

	if (v == 0)
	{
		buf[i++] = '0';
	}
	else 
	{
		if (v < 0)
		{
			negative = true;
			v = -v;
		}
		
		for (; v != 0; v /= 10)
		{
			buf[i++] = (v % 10) + '0';
		}
	}
	
	if (negative)
		buf[i++] = '-';

	while (i < width)
		buf[i++] = fill;
	
	for (; i > 0; --i)
		s.write(buf[i - 1]);
}

}

#endif
