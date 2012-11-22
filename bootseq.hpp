#ifndef AVRLIB_BOOTSEQ_HPP
#define AVRLIB_BOOTSEQ_HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include "nobootseq.hpp"

namespace avrlib {

inline void bootseq_reset()
{
	cli();
#if defined(WDTCR)
# if defined(WDCE)
	WDTCR = (1<<WDCE)|(1<<WDE);
# else
	WDTCR = (1<<WDTOE)|(1<<WDE);
# endif
	WDTCR = (1<<WDE);
#elif defined(WDTCSR)
	WDTCSR = (1<<WDCE)|(1<<WDE);
	WDTCSR = (1<<WDE);
#elif __AVR_ARCH__ >= 100 /*xmega*/
	CCP = CCP_IOREG_gc;
	RST.CTRL = RST_SWRST_bm;
#else
# error Unsupported Watchdog timer interface.
#endif
	for (;;)
	{
	}
}

class zigbit_bootseq
{
public:
	zigbit_bootseq()
		: m_state(0)
	{
	}

	uint8_t check(uint8_t v)
	{
		static uint8_t const seq[] = { 0xB2, 0xA5, 0x65, 0x4B };

		if (seq[m_state++] != v)
			m_state = 0;

		if (m_state == 4)
			bootseq_reset();

		return v;
	}

private:
	uint8_t m_state;
};

class bootseq
{
public:
	bootseq()
		: m_state(0)
	{
	}

	uint8_t check(uint8_t v)
	{
		static uint8_t const seq[] = { 0x74, 0x7E, 0x7A, 0x33 };

		if (seq[m_state++] != v)
			m_state = 0;

		if (m_state == 4)
			bootseq_reset();

		return v;
	}

private:
	uint8_t m_state;
};

template <uint16_t boot_entry_address>
class legacy_bootseq
{
public:
	legacy_bootseq()
		: m_state(0)
	{
	}

	uint8_t check(uint8_t v)
	{
		static uint8_t const seq[] = { 0x74, 0x7E, 0x7A, 0x33 };

		if (seq[m_state++] != v)
			m_state = 0;

		if (m_state == 4)
		{
			void clean();
			clean();

			((void (*)())(void *)boot_entry_address)();
		}

		return v;
	}

private:
	uint8_t m_state;
};

}

#endif
