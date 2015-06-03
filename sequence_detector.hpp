#ifndef SEQUENCE_DETECTOR_HPP
#define SEQUENCE_DETECTOR_HPP

#include "math.hpp"

namespace avrlib
{

template <typename input_t = char, uint8_t max_length = 16>
class sequence_detector_t
{
public:
	sequence_detector_t()
		:m_length(0), m_state(0), m_pass(false)
	{}
		
	sequence_detector_t(const input_t *seqence, const uint8_t &length)
		:m_length(avrlib::clamp(length, uint8_t(0), max_length)),
		 m_state(0), m_pass(false)
	{
		for (uint8_t i = 0; i !=m_length; ++i)
			m_sequence[i] = *(seqence + i);
	}
	
	sequence_detector_t(const input_t *seq_str)
		:m_length(0), m_state(0), m_pass(false)
	{
		for(m_length = 0; m_length != max_length && *seq_str != 0; ++m_length, ++seq_str)
			m_sequence[m_length] = *seq_str;
	}
		
	bool operator () (const input_t &value)
	{
		input(value);
		return pass();
	}

	const input_t& input(const input_t &value)
	{
		if(value == m_sequence[m_state])
		{
			if(++m_state == m_length)
			{
				m_state = 0;
				m_pass = true;
			}
		}
		else
		{
			m_state = 0;
		}
		return value;
	}
	
	void set_sequence(const input_t *seqence, const uint8_t &length)
	{
		m_length = avrlib::clamp(length, uint8_t(0), max_length);
		for (uint8_t i = 0; i !=m_length; ++i)
			m_sequence[i] = *(seqence + i);
	}
	
	void set_sequence(const input_t *seq_str)
	{
		for(m_length = 0; m_length != max_length && *seq_str != 0; ++m_length, ++seq_str)
			m_sequence[m_length] = *seq_str;
	}
	
	bool pass(void)
	{
		if(m_pass)
		{
			m_pass = false;
			return true;
		}
		return false;
	}
	
	operator bool (void)
	{
		return pass();
	}
	
	inline void reset(void)
	{
		m_state = 0;
	}
	
private:
	volatile uint8_t m_length;
	volatile uint8_t m_state;
	volatile bool m_pass;
	input_t m_sequence[max_length];
};

} // namespace avrlib

#endif