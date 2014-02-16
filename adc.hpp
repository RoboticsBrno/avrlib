#ifndef AVRLIB_ADC_HPP
#define AVRLIB_ADC_HPP

#include <avr/io.h>

namespace avrlib {
	
enum adc_timer_mode	
{
	adc_prescaler_2 = 1,
	adc_prescaler_4,
	adc_prescaler_8,	
	adc_prescaler_16,
	adc_prescaler_32,
	adc_prescaler_64,
	adc_prescaler_128	
};

class sync_adc
{
public:
	explicit sync_adc(uint8_t channel, bool reverse = false)
		: m_channel(channel), m_reverse(reverse)
	{
	}
	
	static void init(adc_timer_mode t)
	{
		ADCSRA = (1<<ADEN) | t;		
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

	typedef uint16_t value_type;

	explicit async_adc(uint8_t channel, bool reverse = false)
		: m_channel(channel), m_reverse(reverse), m_value(0), m_new_value(false)
	{
	}

	static void init(adc_timer_mode t)
	{
		ADCSRA = (1<<ADEN) | t;
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
		
		m_new_value = true;
		
		return true;
	}

	value_type value()
	{
		m_new_value = false;
		return m_value;
	}
	
	bool new_value() const { return m_new_value; }
		
	uint8_t channel() const { return m_channel; }

private:
	uint8_t m_channel;
	bool m_reverse;
	volatile value_type m_value;
	volatile bool m_new_value;
};

}

#endif
