#ifndef AVRLIB_ADC_HPP
#define AVRLIB_ADC_HPP

#include <avr/io.h>

namespace avrlib {

class sync_adc
{
public:
	explicit sync_adc(uint8_t channel, bool reverse = false)
		: m_channel(channel), m_reverse(reverse)
	{
	}

	void start()
	{
		ADCSRA |= (1<<ADSC);
	}

	bool running() const
	{
		return (ADCSRA & (1<<ADSC)) != 0;
	}

	uint16_t value() const
	{
		uint16_t res = ADCL;
		res |= ADCH << 8;
		return res;
	}

	uint16_t operator()()
	{
		ADMUX = (1<<ADLAR) | m_channel;
		ADCSRA |= (1<<ADSC);
		while ((ADCSRA & (1<<ADIF)) == 0)
		{
		}
		/*ADCSRA |= (1<<ADSC);
		while ((ADCSRA & (1<<ADIF)) == 0)
		{
		}*/

		uint16_t res = ADCL;
		res |= ADCH << 8;

		if (m_reverse)
			res = -res;

		return res;
	}

private:
	uint8_t m_channel;
	bool m_reverse;
};


class async_adc
{
public:
	explicit async_adc(uint8_t channel, bool reverse = false)
		: m_channel(channel), m_reverse(reverse), m_value(0)
	{
	}

	void start()
	{
		ADMUX = (1<<ADLAR) | m_channel;
		ADCSRA |= (1<<ADSC);
	}

	bool process()
	{
		if ((ADCSRA & (1<<ADSC)) != 0)
			return false;

		m_value = ADCL;
		m_value |= ADCH << 8;

		if (m_reverse)
			m_value = -m_value;

		return true;
	}

	uint16_t value() const
	{
		return m_value;
	}

private:
	uint8_t m_channel;
	bool m_reverse;
	uint16_t m_value;
};

}

#endif
