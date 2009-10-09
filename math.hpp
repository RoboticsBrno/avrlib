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

}

#endif
