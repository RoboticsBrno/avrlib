#ifndef AVRLIB_USART_BASE_HPP
#define AVRLIB_USART_BASE_HPP

namespace avrlib {

namespace detail {

static uint16_t get_ubrr(uint32_t speed)
{
	return uint16_t(F_CPU / (8 * speed) - 1);
}

}

}

#endif
