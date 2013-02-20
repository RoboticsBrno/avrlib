#ifndef AVRLIB_USART_BASE_HPP
#define AVRLIB_USART_BASE_HPP

namespace avrlib {

template <uint32_t speed>
struct ubrr
{
	static uint16_t const value = F_CPU / (8 * speed) - 1;
};

namespace detail {

inline uint16_t get_ubrr(uint32_t speed)
{
	return F_CPU / (8 * speed) - 1;
}

}

}

#endif
