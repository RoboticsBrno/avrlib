#ifndef AVRLIB_BOOTSEQ_HPP
#define AVRLIB_BOOTSEQ_HPP

#include <avr/io.h>
#include <avr/interrupt.h>

namespace avrlib {

class nobootseq
{
public:
	uint8_t check(uint8_t v)
	{
		return v;
	}
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
		{
			cli();
			WDTCR = (1<<WDCE)|(1<<WDE);
			WDTCR = (1<<WDE);
			for (;;)
			{
			}
		}

		return v;
	}

private:
	uint8_t m_state;
};

}

#endif
