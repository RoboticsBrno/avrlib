#ifndef AVRLIB_UTIL_HPP
#define AVRLIB_UTIL_HPP

namespace avrlib {

template <typename T>
struct identity
{
	typedef T type;
};

template <typename T>
T load_acquire(T const volatile & t)
{
	return t;
}

template <typename T>
void store_release(T volatile & t, typename identity<T>::type v)
{
	t = v;
}

template <typename T>
bool parity_check_even(const T &data)
{
	uint8_t counter = 0;
	for(uint8_t i = 0; i != (8 * sizeof data); ++i)
		if(data & (T(1)<<i))
			++counter;
	return ((counter & 1) == 0);
}

template <typename T>
void memcpy(T* target, T* source, uint16_t length)
{
	for(uint16_t i = 0; i != length; ++i)
		*(target+i) = *(source+i);
}

}

#endif
