#ifndef AVRLIB_ASSERT_HPP
#define AVRLIB_ASSERT_HPP

#include <avr/interrupt.h>
#include "bootseq.hpp"

namespace avrlib {

void assertion_failed(char const * message, char const * file, int line);

class trace_mask_t
{
public:
	trace_mask_t()
		: m_mask(0)
	{
	}

	void clear()
	{
		m_mask = 0;
	}

	void set(uint32_t mask = 0xffffffff)
	{
		m_mask = mask;
	}

	uint32_t get() const { return m_mask; }

private:
	uint32_t m_mask;
};

extern trace_mask_t trace_mask;
void trace_triggered(uint8_t const * value, uint8_t len, char const * message, char const * file, int line);

template <typename T>
inline void trace_triggered(T value, char const * message, char const * file, int line)
{
	trace_triggered((uint8_t const *)&value, sizeof(T), message, file, line);
}

namespace detail {

template <typename Usart>
void trace_assert_common_main(char const * header, Usart & usart, uint8_t const * value, uint8_t len, char const * message, char const * file, int line)
{
	cli();

	send(usart, header);
	send(usart, file);
	send(usart, "(");
	send_int(usart, line);
	send(usart, "): ");
	send(usart, message);
	send(usart, " = ");
	for (uint8_t i = 0; i < len; ++i)
	{
		send_hex(usart, value[i], 2);
		usart.write(' ');
	}
	send(usart, "\r\n");

	for (;;)
	{
		usart.process_rx();
		usart.process_tx();

		if (usart.empty())
			continue;

		switch (usart.read())
		{
		case '?':
			send(usart, header);
			send(usart, file);
			send(usart, "(");
			send_int(usart, line);
			send(usart, "): ");
			send(usart, message);
			send(usart, " = ");
			for (uint8_t i = 0; i < len; ++i)
			{
				send_hex(usart, value[i], 2);
				usart.write(' ');
			}
			send(usart, "\r\n");
			break;
		case 'q':
			force_wd_reset();
			break;
		case 't':
			trace_mask.set();
			break;
		case 'C':
			trace_mask.clear();
			// fall through
		case 'c':
			sei();
			return;
		}
	}
}

}

template <typename Usart>
void trace_main(Usart & usart, uint8_t const * value, uint8_t len, char const * message, char const * file, int line)
{
	detail::trace_assert_common_main("trace: ", usart, value, len, message, file, line);
}

template <typename Usart>
void assert_main(Usart & usart, char const * message, char const * file, int line)
{
	trace_mask.set();
	detail::trace_assert_common_main("assertion failed: ", usart, 0, 0, message, file, line);
}

}

#ifndef NDEBUG
#define AVRLIB_ASSERT(x) do { if(!(x)) ::avrlib::assertion_failed(#x, __FILE__, __LINE__); } while(0)
#define AVRLIB_VERIFY(x) AVRLIB_ASSERT(x)
#else
#define AVRLIB_ASSERT(x) (void)0
#define AVRLIB_VERIFY(x) (void)(x)
#endif

#ifndef NDEBUG
#define AVRLIB_TRACE(mask, value) do { if (::avrlib::trace_mask.get() & (mask)) ::avrlib::trace_triggered((value), #value, __FILE__, __LINE__); } while(0)
#define AVRLIB_TRACE_BIN(mask, value, len) do { if (::avrlib::trace_mask.get() & (mask)) ::avrlib::trace_triggered((value), (len), #value, __FILE__, __LINE__); } while(0)
#else
#define AVRLIB_TRACE(mask, value) (void)0
#define AVRLIB_TRACE_BIN(mask, value, len) (void)0
#endif

#ifndef NDEBUG
#define AVRLIB_ASSERT_TRACE(x, value) do { if(!(x)) { ::avrlib::trace_triggered((value), #value, __FILE__, __LINE__); ::avrlib::assertion_failed(#x, __FILE__, __LINE__); } } while(0)
#define AVRLIB_VERIFY_TRACE(x, value) AVRLIB_ASSERT_TRACE(x, value)
#else
#define AVRLIB_ASSERT_TRACE(x, value) (void)0
#define AVRLIB_VERIFY_TRACE(x, value) (void)(x)
#endif

#endif
