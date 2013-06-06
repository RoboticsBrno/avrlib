#ifndef AVRLIB_COMMAND_PARSER_HPP
#define AVRLIB_COMMAND_PARSER_HPP

#include <stdint.h>

namespace avrlib {

class command_parser
{
public:
	enum state_t { bad, ready, simple_command, header, st_data };

	command_parser()
		: m_state(bad), m_cmd(0), m_cmd_size(0), m_size(0)
	{
	}

	void clear()
	{
		m_state = ready;
	}

	uint8_t command() const { return m_cmd; }
	uint8_t size() const { return m_size; }
	uint8_t const * data() const { return m_buffer; }

	uint8_t push_data(uint8_t ch)
	{
		switch (m_state)
		{
		case ready:
			if (ch == 0x80)
			{
				m_state = header;
				break;
			}

			if (ch > 16)
			{
				m_size = 0;
				m_cmd = ch;
				m_state = simple_command;
				break;
			}

			m_state = bad;
			break;

		case simple_command:
			m_state = bad;
			break;

		case header:
			m_cmd_size = ch & 0x0f;
			m_cmd = ch >> 4;

			m_size = 0;
			m_state = m_cmd_size == 0? ready: st_data;
			break;

		case st_data:
			m_buffer[m_size++] = ch;
			m_state = m_cmd_size == m_size? ready: st_data;
			break;

		default:
			;
		}

		if (m_state == bad)
			return 254;

		if (m_state == simple_command || m_state == ready)
			return m_cmd;

		return 255;
	}

	state_t state() const { return m_state; }

	uint8_t operator[](uint8_t index) const { return m_buffer[index]; }

private:
	state_t m_state;

	uint8_t m_cmd;
	uint8_t m_cmd_size;

	uint8_t m_buffer[16];
	uint8_t m_size;
};

template <typename Timer, typename Time = typename Timer::time_type>
class timed_command_parser
	: public command_parser
{
public:
	typedef Time time_type;

	explicit timed_command_parser(Timer const & timer, time_type const & timeout = 20000)
		: m_timer(timer), m_timeout(timeout), m_last_push(timer())
	{
	}

	uint8_t push_data(uint8_t ch)
	{
		time_type const & time = m_timer();
		if (time - m_last_push > m_timeout)
			this->clear();

		m_last_push = time;
		return this->command_parser::push_data(ch);	
	}

private:
	Timer const & m_timer;
	time_type const & m_timeout;

	time_type m_last_push;
};

}

#endif
