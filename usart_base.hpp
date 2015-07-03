#ifndef AVRLIB_USART_BASE_HPP
#define AVRLIB_USART_BASE_HPP

namespace avrlib {

template <uint32_t speed>
struct ubrr
{
	static uint16_t const value = ((F_CPU / (4 * speed) + 1) >> 1) - 1;
};

enum uart_interrupt_priority_t
{
	uart_intr_off = 0,
	uart_intr_lo  = 1,
	uart_intr_med = 2,
	uart_intr_hi  = 3
};

namespace detail {

inline uint16_t get_ubrr(uint32_t speed)
{
	return ((F_CPU / (4 * speed) + 1) >> 1) - 1;
}

}

}

#endif
