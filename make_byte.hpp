#ifndef AVRLIB_MAKE_BYTE_HPP
#define AVRLIB_MAKE_BYTE_HPP

#include <stdint.h>

namespace avrlib {

inline uint8_t make_byte()
{
	return 0;
}

inline uint8_t make_byte(
	bool b0)
{
	return b0;
}

inline uint8_t make_byte(
	bool b0, bool b1)
{
	return b0 | (b1 << 1);
}

inline uint8_t make_byte(
	bool b0, bool b1, bool b2)
{
	return b0 | (b1 << 1) | (b2 << 2);
}

inline uint8_t make_byte(
	bool b0, bool b1, bool b2, bool b3)
{
	return b0 | (b1 << 1) | (b2 << 2) | (b3 << 3);
}

inline uint8_t make_byte(
	bool b0, bool b1, bool b2, bool b3,
	bool b4)
{
	return b0 | (b1 << 1) | (b2 << 2) | (b3 << 3) | (b4 << 4);
}

inline uint8_t make_byte(
	bool b0, bool b1, bool b2, bool b3,
	bool b4, bool b5)
{
	return b0 | (b1 << 1) | (b2 << 2) | (b3 << 3) | (b4 << 4) | (b5 << 5);
}

inline uint8_t make_byte(
	bool b0, bool b1, bool b2, bool b3,
	bool b4, bool b5, bool b6)
{
	return b0 | (b1 << 1) | (b2 << 2) | (b3 << 3) | (b4 << 4) | (b5 << 5) | (b6 << 6);
}

inline uint8_t make_byte(
	bool b0, bool b1, bool b2, bool b3,
	bool b4, bool b5, bool b6, bool b7)
{
	return b0 | (b1 << 1) | (b2 << 2) | (b3 << 3) | (b4 << 4) | (b5 << 5) | (b6 << 6) | (b7 << 7);
}

}

#endif
