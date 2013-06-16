#ifndef AVRLIB_NUMERIC_HPP
#define AVRLIB_NUMERIC_HPP

#include <stdint.h>

namespace avrlib {

typedef uint32_t uint_max_t;

namespace detail {

template <bool Cond, typename True, typename False>
struct mpl_if
{
	typedef True type;
};

template <typename True, typename False>
struct mpl_if<false, True, False>
{
	typedef False type;
};

}

// Gives the smallest unsigned type that can represent Value.
template <uint_max_t Value>
struct least_uint
{
	typedef typename detail::mpl_if<(Value < 0x100), uint8_t,
		typename detail::mpl_if<(Value < 0x10000), uint16_t, uint32_t>::type >::type type;
};

// Gives the fastest unsigned type that can represent Value.
template <uint_max_t Value>
struct fastest_uint
{
	typedef typename least_uint<Value>::type type;
};

}

#endif
