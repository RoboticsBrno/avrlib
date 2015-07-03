#ifndef AVRLIB_COMMAND_PARSER_HPP
#define AVRLIB_COMMAND_PARSER_HPP

#include <stdint.h>

namespace avrlib {

class command_parser
{
public:
	enum state_t { bad, ready, simple_command, header, st_data };

	command_parser()
		: m_state(bad), m_cmd(0), m_cmd_size(0), m_size(0), m_tx_ptr(0), m_err_cnt(0)
	{
	}

	void clear()
	{
		m_state = ready;
	}

	uint8_t command() const { return m_cmd; }
	uint8_t size() const { return m_size; }
	uint8_t const * data() const { return m_rx_buffer; }
	uint16_t error_cnt() const { return m_err_cnt; }
	void clear_error_cnt() { m_err_cnt = 0; }

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
			m_rx_buffer[m_size++] = ch;
			m_state = m_cmd_size == m_size? ready: st_data;
			break;

		default:
			;
		}

		if (m_state == bad)
		{
			++m_err_cnt;
			return 254;
		}

		if (m_state == simple_command || m_state == ready)
			return m_cmd;

		return 255;
	}
	
	template <typename T>
	bool write(const T& v)
	{
		if(m_tx_ptr >= (16 - sizeof(v)))
			return false;
		uint8_t const* begin = reinterpret_cast<uint8_t const*>(&v);
		for(const uint8_t end = m_tx_ptr + sizeof(v); m_tx_ptr != end; ++m_tx_ptr, ++begin)
			m_tx_buffer[m_tx_ptr] = *begin;
		return true;
	}
	
	template <typename Usart>
	void send(Usart& usart, const uint8_t& cmd)
	{
		usart.write(0x80);
		usart.write(((cmd & 0x0F)<<4) | m_tx_ptr);
		for(uint8_t i = 0; i != m_tx_ptr; ++i)
			usart.write(m_tx_buffer[i]);
		m_tx_ptr = 0;
	}

	state_t state() const { return m_state; }

	uint8_t operator[](uint8_t index) const { return m_rx_buffer[index]; }
		
	template <typename T>
	T read(const uint8_t& index) const { return *reinterpret_cast<T*>(m_tx_buffer + index); }
		
	uint8_t* get_rx_buffer() { return m_rx_buffer; }
	uint8_t* get_buffer() { return m_rx_buffer; }
		
	uint8_t* get_tx_buffer() { return m_tx_buffer; }

protected:
	state_t m_state;

	uint8_t m_cmd;
	uint8_t m_cmd_size;

	uint8_t m_rx_buffer[16];
	uint8_t m_tx_buffer[16];
	uint8_t m_size;
	uint8_t m_tx_ptr;
	uint16_t m_err_cnt;
};

class safe_command_parser
	: public command_parser
{
public:
	enum state_t { bad, ready, simple_command, header, st_data, checksum };
		
	safe_command_parser()
		:command_parser(), m_state(ready), m_checksum(0)
	{}
		
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
			m_checksum = ch;
			m_cmd_size = ch & 0x0f;
			m_cmd = ch >> 4;

			m_size = 0;
			m_state = m_cmd_size == 0? checksum: st_data;
			break;

		case st_data:
			m_checksum ^= ch;
			m_rx_buffer[m_size++] = ch;
			m_state = m_cmd_size == m_size? checksum: st_data;
			break;
			
		case checksum:
			if(m_checksum != ch)
			{
				m_state = bad;
				++m_err_cnt;
				return 253;
			}
			m_state = ready;
			break;

		default:
			;
		}

		if (m_state == bad)
		{
			++m_err_cnt;
			return 254;
		}

		if (m_state == simple_command || m_state == ready)
			return m_cmd;

		return 255;
	}
	
	template <typename Usart>
	void send(Usart& usart, const uint8_t& cmd)
	{
		usart.write(0x80);
		uint8_t chks = ((cmd & 0x0F)<<4) | m_tx_ptr;
		usart.write(chks);
		for(uint8_t i = 0; i != m_tx_ptr; ++i)
		{
			chks ^= m_tx_buffer[i];
			usart.write(m_tx_buffer[i]);
		}
		usart.write(chks);
		m_tx_ptr = 0;
	}
	
private:
	state_t m_state;
	uint8_t m_checksum;
};

template <class base_class, typename Timer, typename Time = typename Timer::time_type>
class base_timed_command_parser
	: public base_class
{
public:
	typedef Time time_type;

	explicit base_timed_command_parser(Timer const & timer, time_type const & timeout = 20000)
		: m_timer(timer), m_timeout(timeout), m_last_push(timer())
	{
	}

	uint8_t push_data(uint8_t ch)
	{
		time_type const & time = m_timer();
		if (time - m_last_push > m_timeout)
			this->clear();

		m_last_push = time;
		return this->base_class::push_data(ch);	
	}
	
private:
	Timer const & m_timer;
	time_type m_timeout;

	time_type m_last_push;
};

template <typename Timer, typename Time = typename Timer::time_type> using timed_command_parser = base_timed_command_parser<command_parser, Timer, Time>;
template <typename Timer, typename Time = typename Timer::time_type> using safe_timed_command_parser = base_timed_command_parser<safe_command_parser, Timer, Time>;

}

#endif
