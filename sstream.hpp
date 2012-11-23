#ifndef AVRLIB_SSTREAM_HPP
#define AVRLIB_SSTREAM_HPP

#include "iostream.hpp"
#include "util.hpp"

namespace avrlib
{

struct ios_base
{
	typedef int8_t streamoff;
	typedef uint8_t streampos;
	typedef int8_t seekdir;
	
	static const seekdir beg =  1;
	static const seekdir cur =  0;
	static const seekdir end = -1;
	
	virtual void write(const char &ch) const {}
};

template <typename T, AVRLIB_STRING_SIZE_TYPE _alloc_space>
class isstream
	:public ios_base, public istream_t<isstream<T, _alloc_space> >
{
public:

	typedef basic_string<T, _alloc_space> buff_type;
	
	isstream(buff_type &buff)
		:istream_t<isstream>(*this), m_buff(buff), m_p(0)
	{}
		
	operator buff_type&()
	{
		return m_buff;
	}
	
	streampos tellg(void)
	{
		return m_p;
	}
	
	void seekg(const streampos &pos)
	{
		m_p = min(pos, m_buff.length());
	}
	
//******************* FIX ME *************************
// 	void seekg(const streamoff &off, const seekdir &dir)
// 	{
// 		streampos temp;
// 		switch(dir)
// 		{
// 		case beg:
// 			if()
// 			m_p = max(m_p + off, m_buff.length());
// 			break;
// 			
// 		case end:
// 			
// 	}

//protected:
	
	bool peek(T &ch)
	{
		if(m_p < m_buff.size())
		{
			ch = m_buff[m_p++];
			return true;
		}
		return false;
	}
	
	T get(void)
	{
		T temp;
		if(peek(temp))
			return temp;
		return '\r';
	}
	
private:
	
	buff_type &m_buff;
	streampos m_p;
};

template <typename T, AVRLIB_STRING_SIZE_TYPE _alloc_space>
class osstream
	:public ios_base, public ostream_t<osstream<T, _alloc_space> >
{
public:

	typedef basic_string<T, _alloc_space> buff_type;

	osstream(buff_type &buff)
		:ostream_t<osstream>(*this), m_buff(buff)
	{}
		
	operator buff_type&()
	{
		return m_buff;
	}

//protected:

	void write(const T &ch)
	{
		m_buff += ch;
	}

	void wait(void)
	{}
		
private:
	
	buff_type &m_buff;
};

template <typename T, AVRLIB_STRING_SIZE_TYPE _alloc_space>
class basic_sstream
	:public isstream<T, _alloc_space>, public osstream<T, _alloc_space>
{
public:

	typedef basic_string<T, _alloc_space> buff_type;
	
	basic_sstream(buff_type &buff)
		:isstream<T, _alloc_space>(buff), osstream<T, _alloc_space>(buff)
	{}
};

typedef basic_sstream<char, AVRLIB_DEFAULT_STRING_SIZE> sstream;

}// end of namespace avrlib

#endif