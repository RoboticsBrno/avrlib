#ifndef AVRLIB_SERIALIZE_HPP
#define AVRLIB_SERIALIZE_HPP

#include "numeric.hpp"

namespace avrlib {

template <typename T>
void serialize(uint8_t * p, T const & t)
{
	uint8_t const * q = reinterpret_cast<uint8_t const *>(&t);
	for (typename fastest_uint<sizeof(T)>::type i = sizeof(T); i != 0; --i)
		*p++ = *q++;
}

template <typename T>
void deserialize(T & t, uint8_t const * p)
{
	uint8_t * q = reinterpret_cast<uint8_t *>(&t);
	for (typename fastest_uint<sizeof(T)>::type i = sizeof(T); i != 0; --i)
		*q++ = *p++;
}

template <typename T>
T deserialize(uint8_t const * p)
{
	T t[1] = {};
	deserialize(t[0], p);
	return t[0];
}

} // namespace avrlib

#endif // AVRLIB_SERIALIZE_HPP
