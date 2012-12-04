#ifndef AVRLIB_MATH_HPP
#define AVRLIB_MATH_HPP

#include "util.hpp"

namespace avrlib {

template <typename T>
T clamp(T value, typename identity<T>::type min, typename identity<T>::type max)
{
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

template <typename T>
T abs(const T &v) {	return v<0?-v:v; }

template <typename T>
T sign(const T &v)
{
	if (v > 0)
		return 1;
	else if (v < 0)
		return -1;
	else
		return 0;
}

template <typename T>
T pow(T base, uint8_t exp = 2)
{
	T res = base;
	if(exp == 0)
		return 1;
	while(--exp != 0)
		res *= base;
	return res;
}

}

#endif
