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
void store_release(T volatile & t, T v)
{
	t = v;
}

}

#endif
