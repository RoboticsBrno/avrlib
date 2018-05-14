#ifndef AVRLIB_USART_BASE_HPP
#define AVRLIB_USART_BASE_HPP

namespace avrlib {

#if __AVR_ARCH__ >= 100 /*xmega*/

enum uart_data_bits_t
{
	uart_5_bits = USART_CHSIZE_5BIT_gc,
	uart_6_bits = USART_CHSIZE_6BIT_gc,
	uart_7_bits = USART_CHSIZE_7BIT_gc,
	uart_8_bits = USART_CHSIZE_8BIT_gc,
	uart_9_bits = USART_CHSIZE_9BIT_gc
};

enum uart_parity_t
{
	uart_no_parity = USART_PMODE_DISABLED_gc,
	uart_even_parity = USART_PMODE_EVEN_gc,
	uart_odd_parity = USART_PMODE_ODD_gc
};

#else /*mega*/

enum uart_data_bits_t
{
	uart_5_bits = 0,
	uart_6_bits = 1,
	uart_7_bits = 2,
	uart_8_bits = 3,
	uart_9_bits = 7
};

#ifndef UPM00
#define UPM00 UPM0
#define UPM01 UPM1
#endif

enum uart_parity_t
{
	uart_no_parity   = (0<<UPM01) | (0<<UPM00),
	uart_even_parity = (1<<UPM01) | (0<<UPM00),
	uart_odd_parity  = (1<<UPM01) | (1<<UPM00)
};

template <uint32_t speed>
struct ubrr
{
	static uint16_t const value = ((F_CPU / (4 * speed) + 1) >> 1) - 1;
};

namespace detail {

inline uint16_t get_ubrr(uint32_t speed)
{
	return ((F_CPU / (4 * speed) + 1) >> 1) - 1;
}

}

#endif /* mega / xmega */

enum uart_interrupt_priority_t
{
	uart_intr_off = 0,
	uart_intr_lo  = 1,
	uart_intr_med = 2,
	uart_intr_hi  = 3
};

}

#endif
