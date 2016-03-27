#ifndef AVRLIB_STRING_HPP
#define AVRLIB_STRING_HPP

namespace avrlib {

#ifndef AVRLIB_DEFAULT_STRING_SIZE
#define AVRLIB_DEFAULT_STRING_SIZE 32
#endif

#ifndef AVRLIB_STRING_SIZE_TYPE 
#define AVRLIB_STRING_SIZE_TYPE uint8_t
#endif

#if __cplusplus <= 199711L
	#define AVRLIB_STRING_NULLPTR NULL
#else
	#define AVRLIB_STRING_NULLPTR nullptr
#endif

// #ifndef npos
// #define npos ((AVRLIB_STRING_SIZE_TYPE) -1)
// #endif

template <typename T, AVRLIB_STRING_SIZE_TYPE _alloc_space>
class basic_string {
	
public:

	typedef AVRLIB_STRING_SIZE_TYPE size_t;
	typedef T CHAR;
	
	static const size_t npos = -1;
	
//constructor

	basic_string()
		: _length(0)
	{}
		
	basic_string(const basic_string<T, _alloc_space>& str)
		: _length(0)
	{
		assign(str);
	}
	
	basic_string(const volatile basic_string<T, _alloc_space>& str)
		: _length(0)
	{
		assign(str);
	}
	
	basic_string(const basic_string<T, _alloc_space>& str, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n)
		: _length(0)
	{
		assign(str, pos, n);
	}
	
	basic_string(const T* s, AVRLIB_STRING_SIZE_TYPE n)
		: _length(0)
	{
		assign(s, n);
	}
	
	basic_string(const T* s)
		: _length(0)
	{
		assign(s);
	}
	
	basic_string(AVRLIB_STRING_SIZE_TYPE n, T c)
		: _length(0)
	{
		assign(n, c);
	}
	
//pointers

	inline T* begin()
	{
		return m_data;
	}
	
	inline const T* begin() const
	{
		return m_data;
	}
	
	inline T* end()
	{
		return m_data + _length;
	}
	
	inline const T* end() const
	{
		return m_data + _length;
	}
	
//capacity

	inline AVRLIB_STRING_SIZE_TYPE size() const
	{
		return _length;
	}
	
	inline AVRLIB_STRING_SIZE_TYPE size() const volatile
	{
		return _length;
	}
	
	inline AVRLIB_STRING_SIZE_TYPE length() const
	{
		return _length;
	}
	
	inline AVRLIB_STRING_SIZE_TYPE max_size() const
	{
		return _alloc_space;
	}
	
	inline void clear()
	{
		_length = 0;
	}
	
	inline bool empty() const
	{
		return _length == 0;
	}
	
//element access

	inline const T& operator[](AVRLIB_STRING_SIZE_TYPE index) const
	{
		return m_data[index];
	}
	
	inline const T& operator[](AVRLIB_STRING_SIZE_TYPE index) const volatile
	{
		return *const_cast<T*>(m_data+index);
	}

	inline T& operator[](AVRLIB_STRING_SIZE_TYPE index)
	{
		return m_data[index];
	}
	
	const T& at(AVRLIB_STRING_SIZE_TYPE index) const
	{
		if(index < _length)
			return m_data[index];
		return m_data[(_length==0?0:_length)-1];
	}

	T& at(AVRLIB_STRING_SIZE_TYPE index)
	{
		if(index < _length)
			return m_data[index];
		return m_data[(_length==0?0:_length)-1];
	}

//append

	basic_string<T, _alloc_space>& append(const basic_string<T, _alloc_space>& str)
	{
		AVRLIB_STRING_SIZE_TYPE chars_count = (_alloc_space - _length) < str.size()? (_alloc_space - _length) : str.size();
		for (AVRLIB_STRING_SIZE_TYPE i = 0; i != chars_count; ++i, ++_length)
			m_data[_length] = str[i];
		return *this;
	}
	
	basic_string<T, _alloc_space>& append(const basic_string<T, _alloc_space>& str, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n)
	{
		return append(str.substr(pos, n));
	}//FIX ME - should be optimised
	
	basic_string<T, _alloc_space>& append(const T* s, AVRLIB_STRING_SIZE_TYPE n)
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return *this;
		n = (_alloc_space - _length) < n? (_alloc_space - _length) : n; 
		for(AVRLIB_STRING_SIZE_TYPE i = 0; i != n; ++i, ++_length)
			m_data[_length] = *(s + i);
		return *this;
	}
	
	basic_string<T, _alloc_space>& append(const T* s)
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return *this;
		for(; _length != _alloc_space && *s != 0; ++s, ++_length)
			m_data[_length] = *s;
		return *this;
	}
	
	basic_string<T, _alloc_space>& append(AVRLIB_STRING_SIZE_TYPE n, T c)
	{
		n = (_alloc_space - _length) < n? (_alloc_space - _length) : n;
		for (AVRLIB_STRING_SIZE_TYPE i = 0; i != n; ++i, ++_length)
			m_data[_length] = c;
		return *this;
	}
	
	
	void push_back(T c)
	{
		if (_length == _alloc_space)
			return;
		m_data[_length] = c;
		++_length;
	}
	
	inline void write(T c) { push_back(c); } // non-standard method, added because avrlib::format
	
	
	inline basic_string<T, _alloc_space>& operator += (const basic_string<T, _alloc_space>& str)
	{
		append(str);
		return *this;
	}
	
	inline basic_string<T, _alloc_space>& operator += (const T* s)
	{
		append(s);
		return *this;
	}
	
	inline basic_string<T, _alloc_space>& operator += (T c)
	{
		push_back(c);
		return *this;
	}
	
//assign

	basic_string<T, _alloc_space>& assign(const basic_string<T, _alloc_space>& str)
	{
		AVRLIB_STRING_SIZE_TYPE chars_count = _alloc_space < str.size()? _alloc_space : str.size();
		for (_length = 0; _length != chars_count; ++_length)
			m_data[_length] = str[_length];
		return *this;
	}
	
	basic_string<T, _alloc_space>& assign(const volatile basic_string<T, _alloc_space>& str) volatile
	{
		AVRLIB_STRING_SIZE_TYPE chars_count = _alloc_space < str.size()? _alloc_space : str.size();
		for (_length = 0; _length != chars_count; ++_length)
			m_data[_length] = str[_length];
		return *const_cast<basic_string<T, _alloc_space>*>(this);
	}
	
	basic_string<T, _alloc_space>& assign(const basic_string<T, _alloc_space>& str, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n)
	{
		return assign(str.substr(pos, n));
	}//FIX ME - should be optimised
	
	basic_string<T, _alloc_space>& assign(const T* s, AVRLIB_STRING_SIZE_TYPE n)
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return *this;
		n = _alloc_space < n? _alloc_space : n;
		for (_length = 0; _length != n; ++_length, ++s)
			m_data[_length] = *s;
		return *this;
	}
	
	basic_string<T, _alloc_space>& assign(const T* s)
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return *this;
		for(_length = 0; _length != _alloc_space && *s != 0; ++s, ++_length)
			m_data[_length] = *s;
		return *this;
	}
	
	basic_string<T, _alloc_space>& assign(AVRLIB_STRING_SIZE_TYPE n, T c)
	{
		n = _alloc_space < n? _alloc_space : n;
		for (_length = 0; _length != n; ++_length)
			m_data[_length] = c;
		return *this;
	}
	
	
	inline basic_string<T, _alloc_space>& operator = (const basic_string<T, _alloc_space>& str)
	{
		assign(str);
		return *this;
	}
	
	inline basic_string<T, _alloc_space>& operator = (const volatile basic_string<T, _alloc_space>& str) volatile
	{
		assign(str);
		return *const_cast<basic_string<T, _alloc_space>*>(this);
	}
	
	inline basic_string<T, _alloc_space>& operator = (const T* s)
	{
		assign(s);
		return *this;
	}
	
	basic_string<T, _alloc_space>& operator = (const T c)
	{
		m_data[0] = c;
		_length = 1;
		return *this;
	}
	
//erase

	basic_string<T, _alloc_space>& erase(AVRLIB_STRING_SIZE_TYPE pos = 0, AVRLIB_STRING_SIZE_TYPE n = npos)
	{
		if (pos >= _length)
			return *this;
		if ((pos + n) >= _length || n == npos)
		{
			_length = pos;
			return *this;
		}			
		for(; pos != (_length - n); ++pos)
			m_data[pos] = m_data[pos + n];
		_length -= n;
		return *this;
	}
	
	T* erase(T* position)
	{
		if (position < begin() || position >= end())
			return end();
		for(AVRLIB_STRING_SIZE_TYPE i = (position - begin()); i != (_length - 1); ++i)
			m_data[i] = m_data[i + 1];
		--_length;
		return position;
	}
	
	T* erase(T* first, T* last)
	{
		if (first < begin())
			return end();
		if (last > end())
			last = end();
		AVRLIB_STRING_SIZE_TYPE n = last - first + 1;
		for(AVRLIB_STRING_SIZE_TYPE i = (first - begin()); i != (_length - n); ++i)
			m_data[i] = m_data[i + n];
		_length -= n;
		return first;
	}
	
//insert
//if sum of both strings is too long, end of final string will be cut off
//NOT OPTIMALIZED!!!

	basic_string<T, _alloc_space>& insert(AVRLIB_STRING_SIZE_TYPE pos1, const basic_string<T, _alloc_space>& str)
	{
		basic_string<T, _alloc_space> temp = substr(pos1);
		erase(pos1);
		append(str);
		append(temp);
		return *this;
	}
	
	basic_string<T, _alloc_space>& insert(AVRLIB_STRING_SIZE_TYPE pos1, const basic_string<T, _alloc_space>& str, AVRLIB_STRING_SIZE_TYPE pos2, AVRLIB_STRING_SIZE_TYPE n)
	{
		basic_string<T, _alloc_space> temp = substr(pos1);
		erase(pos1);
		append(str.substr(pos2, n));
		append(temp);
		return *this;
	}
	
	basic_string<T, _alloc_space>& insert(AVRLIB_STRING_SIZE_TYPE pos1, const T* s, AVRLIB_STRING_SIZE_TYPE n)
	{
		basic_string<T, _alloc_space> temp = substr(pos1);
		erase(pos1);
		append(s, n);
		append(temp);
		return *this;
	}
	
	basic_string<T, _alloc_space>& insert(AVRLIB_STRING_SIZE_TYPE pos1, const T* s)
	{
		basic_string<T, _alloc_space> temp = substr(pos1);
		erase(pos1);
		append(s);
		append(temp);
		return *this;
	}
	
	basic_string<T, _alloc_space>& insert(AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n, T c)
	{
		basic_string<T, _alloc_space> temp = substr(pos1);
		erase(pos1);
		append(n, c);
		append(temp);
		return *this;
	}
	
//replace
//if sum of non replaced part of this strings and substituting string is too long, end of final string will be cut off
//NOT OPTIMISED!!!

	basic_string<T, _alloc_space>& replace ( AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n1,   const basic_string<T, _alloc_space>& str )
	{
		erase(pos1, n1);
		insert(pos1, str);
		return *this;
	}
	
	basic_string<T, _alloc_space>& replace ( T* i1, T* i2, const basic_string<T, _alloc_space>& str )
	{
		erase(i1, i2 - 1);
		insert(i1 - begin(), str);
		return *this;
	}
	
	basic_string<T, _alloc_space>& replace ( AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n1, const basic_string<T, _alloc_space>& str, AVRLIB_STRING_SIZE_TYPE pos2, AVRLIB_STRING_SIZE_TYPE n2 )
	{
		erase(pos1, n1);
		insert(pos1, str, pos2, n2);
		return *this;
	}
	
	basic_string<T, _alloc_space>& replace ( AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n1,   const T* s, AVRLIB_STRING_SIZE_TYPE n2 )
	{
		erase(pos1, n1);
		insert(pos1, s, n2);
		return *this;
	}
	
	basic_string<T, _alloc_space>& replace ( T* i1, T* i2, const T* s, AVRLIB_STRING_SIZE_TYPE n2 )
	{
		erase(i1, i2 - 1);
		insert(i1 - begin(), s, n2);
		return *this;
	}
	
	basic_string<T, _alloc_space>& replace ( AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n1,   const T* s )
	{
		erase(pos1, n1);
		insert(pos1, s);
		return *this;
	}
	
	basic_string<T, _alloc_space>& replace ( T* i1, T* i2, const T* s )
	{
		erase(i1, i2 - 1);
		insert(i1 - begin(), s);
		return *this;
	}
	
	basic_string<T, _alloc_space>& replace ( AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n1,   AVRLIB_STRING_SIZE_TYPE n2, T c )
	{
		erase(pos1, n1);
		insert(pos1, n2, c);
		return *this;
	}
	
	basic_string<T, _alloc_space>& replace ( T* i1, T* i2, AVRLIB_STRING_SIZE_TYPE n2, T c )
	{
		erase(i1, i2 - 1);
		insert(i1 - begin(), n2, c);
		return *this;
	}
	
//swap
//NOT OPTIMISED!!!

	void swap (basic_string<T, _alloc_space>& str)
	{
		basic_string<T, _alloc_space> temp = str;
		str = *this;
		assign(temp);
	}
	
//data

	const T* data() const
	{
		return m_data;
	}
	
//copy

	AVRLIB_STRING_SIZE_TYPE copy( T* s, AVRLIB_STRING_SIZE_TYPE n, AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return 0;
		n = n > (_length - pos) ? (_length - pos) : n;
		for(AVRLIB_STRING_SIZE_TYPE i = 0; i != n; ++i)
			*(s + i) = m_data[pos + i];
		return n;
	}
	
//find
	
	AVRLIB_STRING_SIZE_TYPE find(const basic_string<T, _alloc_space>& str,  AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		if (str.empty() || pos >= _length || str.length() > _length || str.length() > (_length - pos))
			return npos;
		for(;;)
		{
			for(; pos != _length; ++pos)
				if (m_data[pos] == str[0])
					break;
			if (str.length() > (_length - pos))
				return npos;
			for(AVRLIB_STRING_SIZE_TYPE j = 1; m_data[pos + j] == str[j]; ++j)
				if (j == str.length())
					return pos;
			++pos;
		}
	}
	
	AVRLIB_STRING_SIZE_TYPE find(const T* s, AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		if(pos >= _length || s == AVRLIB_STRING_NULLPTR || s[0] == 0)
			return npos;
		for(;;)
		{
			for(; pos != _length; ++pos)
				if (m_data[pos] == s[0])
					break;
			if (pos == _length)
				return npos;
			AVRLIB_STRING_SIZE_TYPE j = 1;
			for(; (pos + j) != _length && m_data[pos + j] == s[j] && s[j] != 0; ++j);
			if (s[j] == 0)
				return pos;
			++pos;
		}
	}
	
	AVRLIB_STRING_SIZE_TYPE find(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		if(s == AVRLIB_STRING_NULLPTR || n == 0 || pos >= _length || n > _length || n > (_length - pos))
			return npos;
		for(;;)
		{
			for(; pos != _length; ++pos)
				if (m_data[pos] == s[0])
					break;
			if (n > (_length - pos))
				return npos;
			for(AVRLIB_STRING_SIZE_TYPE j = 1; m_data[pos + j] == s[j]; ++j)
				if (j == n)
					return pos;
			++pos;
		}
	}
	
	AVRLIB_STRING_SIZE_TYPE find(const T c,  AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		for(; pos < _length; ++pos)
			if (m_data[pos] == c)
				return pos;
		return npos;
	}
	
//rfind
	
	AVRLIB_STRING_SIZE_TYPE rfind(const basic_string<T, _alloc_space>& str,  AVRLIB_STRING_SIZE_TYPE pos = npos) const
	{
		if (str.empty() || str.length() > _length)
			return npos;
		if (pos > (_length - str.length()))
			pos = (_length - str.length());
		++pos;
		for(;;)
		{
			for(; pos != 0; --pos)
				if (m_data[pos - 1] == str[0])
					break;
			if (pos == 0)
				return npos;
			for(AVRLIB_STRING_SIZE_TYPE j = 1; m_data[pos + j - 1] == str[j]; ++j)
				if (j == str.length())
					return pos - 1;
			--pos;
		}
	}
	
	AVRLIB_STRING_SIZE_TYPE rfind(const T* s, AVRLIB_STRING_SIZE_TYPE pos = npos) const
	{
		if(s == AVRLIB_STRING_NULLPTR || s[0] == 0)
			return npos;
		AVRLIB_STRING_SIZE_TYPE len = strlen(s);
		if (len > _length)
			return npos;
		if (pos > (_length - len))
			pos = (_length - len);
		++pos;
		for(;;)
		{
			for(; pos != 0; --pos)
				if (m_data[pos - 1] == s[0])
					break;
			if (pos == 0)
				return npos;
			for(AVRLIB_STRING_SIZE_TYPE j = 1; m_data[pos + j - 1] == s[j]; ++j)
				if (j == len)
					return pos - 1;
			--pos;
		}
	}
	
	AVRLIB_STRING_SIZE_TYPE rfind(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		if(s == AVRLIB_STRING_NULLPTR || s[0] == 0)
			return npos;
		if (n > _length)
			return npos;
		if (pos > (_length - n))
			pos = (_length - n);
		++pos;
		for(;;)
		{
			for(; pos != 0; --pos)
				if (m_data[pos - 1] == s[0])
					break;
			if (pos == 0)
				return npos;
			for(AVRLIB_STRING_SIZE_TYPE j = 1; m_data[pos + j - 1] == s[j]; ++j)
				if (j == n)
					return pos - 1;
			--pos;
		}
	}
	
	AVRLIB_STRING_SIZE_TYPE rfind(const T c,  AVRLIB_STRING_SIZE_TYPE pos = npos) const
	{
		if (pos > _length)
			pos = _length;
		else
			++pos;
		for(; pos != 0; --pos)
			if (m_data[pos - 1] == c)
				return pos - 1;
		return npos;
	}
	
//find_first_of

	AVRLIB_STRING_SIZE_TYPE find_first_of(const basic_string<T, _alloc_space>& str,  AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		if (str.length() == 0)
			return npos;
		for(; pos < _length; ++pos)
			for(AVRLIB_STRING_SIZE_TYPE j = 0; j != str.length(); ++j)
				if (m_data[pos] == str[j])
					return pos;
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_first_of(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		if (s == AVRLIB_STRING_NULLPTR || n == 0)
			return npos;
		for(; pos < _length; ++pos)
			for(AVRLIB_STRING_SIZE_TYPE j = 0; j != n; ++j)
				if (m_data[pos] == *(s + j))
					return pos;
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_first_of(const T* s, AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return npos;
		for(; pos < _length; ++pos)
			for(AVRLIB_STRING_SIZE_TYPE j = 0; *(s + j) != 0; ++j)
				if (m_data[pos] == *(s + j))
					return pos;
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_first_of(const T c,  AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		for(; pos < _length; ++pos)
			if (m_data[pos] == c)
				return pos;
		return npos;
	}
	
//find_last_of

	AVRLIB_STRING_SIZE_TYPE find_last_of(const basic_string<T, _alloc_space>& str,  AVRLIB_STRING_SIZE_TYPE pos = npos) const
	{
		
		if (str.length() == 0)
			return npos;
		if (pos > _length)
			pos = _length;
		else
			++pos;
		for(; pos != 0; --pos)
			for(AVRLIB_STRING_SIZE_TYPE j = 0; j != str.length(); ++j)
				if (m_data[pos - 1] == str[j])
					return pos - 1;
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_last_of(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		if (s == AVRLIB_STRING_NULLPTR || n == 0)
			return npos;
		if (pos > _length)
			pos = _length;
		else
			++pos;
		for(; pos != 0; --pos)
			for(AVRLIB_STRING_SIZE_TYPE j = 0; j != n; ++j)
				if (m_data[pos - 1] == *(s + j))
					return pos - 1;
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_last_of(const T* s, AVRLIB_STRING_SIZE_TYPE pos = npos) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return npos;
		if (pos > _length)
			pos = _length;
		else
			++pos;
		for(; pos != 0; --pos)
			for(AVRLIB_STRING_SIZE_TYPE j = 0; *(s + j) != 0; ++j)
				if (m_data[pos - 1] == *(s + j))
					return pos - 1;
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_last_of(const T c,  AVRLIB_STRING_SIZE_TYPE pos = npos) const
	{
		if (pos > _length)
			pos = _length;
		else
			++pos;
		for(; pos != 0; --pos)
			if (m_data[pos - 1] == c)
				return pos - 1;
		return npos;
	}
	
//find_first_not_of

	AVRLIB_STRING_SIZE_TYPE find_first_not_of(const basic_string<T, _alloc_space>& str,  AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		if (str.length() == 0)
			return npos;
		bool braked = false;
		for(; pos < _length; ++pos)
		{
			for(AVRLIB_STRING_SIZE_TYPE j = 0; j != str.length(); ++j)
			{
				if (m_data[pos] == str[j])
				{
					braked = true;
					break;
				}				
			}
			if (!braked)
				return pos;
			braked = false;
		}						
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_first_not_of(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		if (s == AVRLIB_STRING_NULLPTR || n == 0)
			return npos;
		bool braked = false;
		for(; pos < _length; ++pos)
		{
			for(AVRLIB_STRING_SIZE_TYPE j = 0; j != n; ++j)
			{
				if (m_data[pos] == *(s + j))
				{
					braked = true;
					break;
				}				
			}
			if (!braked)
				return pos;
			braked = false;
		}						
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_first_not_of(const T* s, AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return npos;
		bool braked = false;
		for(; pos < _length; ++pos)
		{
			for(AVRLIB_STRING_SIZE_TYPE j = 0; *(s + j) != 0; ++j)
			{
				if (m_data[pos] == *(s + j))
				{
					braked = true;
					break;
				}				
			}
			if (!braked)
				return pos;
			braked = false;
		}						
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_first_not_of(const T c,  AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		for(; pos < _length; ++pos)
			if (m_data[pos] != c)
				return pos;
		return npos;
	}
	
//find_last_not_of

	AVRLIB_STRING_SIZE_TYPE find_last_not_of(const basic_string<T, _alloc_space>& str,  AVRLIB_STRING_SIZE_TYPE pos = npos) const
	{
		if (str.length() == 0)
			return npos;
		bool braked = false;
		if (pos > _length)
			pos = _length;
		else
			++pos;
		for(; pos != 0; --pos)
		{
			for(AVRLIB_STRING_SIZE_TYPE j = 0; j != str.length(); ++j)
			{
				if (m_data[pos - 1] == str[j])
				{
					braked = true;
					break;
				}				
			}
			if (!braked)
				return pos - 1;
			braked = false;
		}						
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_last_not_of(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		if (s == AVRLIB_STRING_NULLPTR || n == 0)
			return npos;
		bool braked = false;
		if (pos > _length)
			pos = _length;
		else
			++pos;
		for(; pos != 0; --pos)
		{
			for(AVRLIB_STRING_SIZE_TYPE j = 0; j != n; ++j)
			{
				if (m_data[pos - 1] == *(s + j))
				{
					braked = true;
					break;
				}				
			}
			if (!braked)
				return pos - 1;
			braked = false;
		}						
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_last_not_of(const T* s, AVRLIB_STRING_SIZE_TYPE pos = npos) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return npos;
		bool braked = false;
		if (pos > _length)
			pos = _length;
		else
			++pos;
		for(; pos != 0; --pos)
		{
			for(AVRLIB_STRING_SIZE_TYPE j = 0; *(s + j) != 0; ++j)
			{
				if (m_data[pos - 1] == *(s + j))
				{
					braked = true;
					break;
				}				
			}
			if (!braked)
				return pos - 1;
			braked = false;
		}						
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_last_not_of(const T c,  AVRLIB_STRING_SIZE_TYPE pos = npos) const
	{
		if (pos > _length)
			pos = _length;
		else
			++pos;
		for(; pos != 0; --pos)
			if (m_data[pos - 1] != c)
				return pos - 1;
		return npos;
	}

//substr

	basic_string<T, _alloc_space> substr(AVRLIB_STRING_SIZE_TYPE pos = 0, AVRLIB_STRING_SIZE_TYPE n = npos) const
	{
		basic_string<T, _alloc_space> ret;
		for(; pos < _length && n != 0; ++pos, --n)
			ret.push_back(m_data[pos]);
		return ret;
	}
	
//compare

	int8_t compare ( const basic_string<T, _alloc_space>& str ) const
	{
		if (_length < str.length())
			return -1;
		if (_length > str.length())
			return 1;
		for(AVRLIB_STRING_SIZE_TYPE i = 0; i != _length; ++i)
		{
			if (m_data[i] == str[i])
				continue;
			if (m_data[i] < str[i])
				return -1;
			if (m_data[i] > str[i])
				return 1;
		}
		return 0;
	}
	
	int8_t compare ( const T* s ) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return empty() ? 0 : 1;
		AVRLIB_STRING_SIZE_TYPE i = 0;
		for(; i != _length; ++i)
		{
			if (*(s + i) == 0)
				return 1;
			if (m_data[i] == *(s + i))
				continue;
			if (m_data[i] < *(s + i))
				return -1;
			if (m_data[i] > *(s + i))
				return 1;
		}
		if (*(s + i) == 0)
			return 0;
		return -1;
	}
	
	int8_t compare ( AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n1, const basic_string<T, _alloc_space>& str ) const
	{
		n1 = (pos1 + n1) <= _length? n1 : (_length - pos1);
		if (n1 < str.length())
			return -1;
		if (n1 > str.length())
			return 1;
		for(AVRLIB_STRING_SIZE_TYPE i = 0; i != n1; ++i)
		{
			if (m_data[i + pos1] == str[i])
				continue;
			if (m_data[i + pos1] < str[i])
				return -1;
			if (m_data[i + pos1] > str[i])
				return 1;
		}
		return 0;
	}
	
	int8_t compare ( AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n1, const T* s) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return empty() ? 0 : 1;
		n1 = (pos1 + n1) <= _length? n1 : (_length - pos1);
		AVRLIB_STRING_SIZE_TYPE i = 0;
		for(; i != n1; ++i)
		{
			if (*(s + i) == 0)
				return 1;
			if (m_data[i + pos1] == *(s + i))
				continue;
			if (m_data[i + pos1] < *(s + i))
				return -1;
			if (m_data[i + pos1] > *(s + i))
				return 1;
		}
		if (*(s + i) == 0)
			return 0;
		return -1;
	}
	
	int8_t compare ( AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n1, const basic_string<T, _alloc_space>& str, AVRLIB_STRING_SIZE_TYPE pos2, AVRLIB_STRING_SIZE_TYPE n2 ) const
	{
		n1 = (pos1 + n1) <= _length? n1 : (_length - pos1);
		n2 = (pos2 + n2) <= str.length()? n2 : (str.length() - pos2);
		if (n1 < n2)
			return -1;
		if (n1 > n2)
			return 1;
		for(AVRLIB_STRING_SIZE_TYPE i = 0; i != n1; ++i)
		{
			if (m_data[i + pos1] == str[i + pos2])
				continue;
			if (m_data[i + pos1] < str[i + pos2])
				return -1;
			if (m_data[i + pos1] > str[i + pos2])
				return 1;
		}
		return 0;
	}
	
	int8_t compare ( AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n1, const T* s, AVRLIB_STRING_SIZE_TYPE n2) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return empty() ? 0 : 1;
		n1 = (pos1 + n1) <= _length? n1 : (_length - pos1);
		if (n1 < n2)
			return -1;
		if (n1 > n2)
			return 1;
		for(AVRLIB_STRING_SIZE_TYPE i = 0; i != n1; ++i)
		{
			if (m_data[i + pos1] == *(s + i))
				continue;
			if (m_data[i + pos1] < *(s + i))
				return -1;
			if (m_data[i + pos1] > *(s + i))
				return 1;
		}
		return 0;
	}

// auxiliary static methods

	static AVRLIB_STRING_SIZE_TYPE strlen(const T* s)
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return 0;
		AVRLIB_STRING_SIZE_TYPE n = 0;
		for (; *s != 0; ++s)
			++n;
		return n;
	}
	
#ifdef __PGMSPACE_H_

//constructor

	struct PGM {};

	basic_string(const T* s, AVRLIB_STRING_SIZE_TYPE n, PGM)
		: _length(0)
	{
		assign_spgm(s, n);
	}
	
	basic_string(const T* s, PGM)
		: _length(0)
	{
		assign_spgm(s);
	}

// append

	basic_string<T, _alloc_space>& append_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE n)
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return *this;
		n = (_alloc_space - _length) < n? (_alloc_space - _length) : n;
		for(AVRLIB_STRING_SIZE_TYPE i = 0; i != n; ++i, ++_length)
			m_data[_length] = pgm_read_elem(s + i);
		return *this;
	}
	
	basic_string<T, _alloc_space>& append_spgm(const T* s)
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return *this;
		for(; _length != _alloc_space && *s != 0; ++s, ++_length)
			m_data[_length] = pgm_read_elem(s);
		return *this;
	}
	
// assign

	basic_string<T, _alloc_space>& assign_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE n)
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return *this;
		n = _alloc_space < n? _alloc_space : n;
		for (_length = 0; _length != n; ++_length, ++s)
			m_data[_length] = pgm_read_elem(s);
		return *this;
	}
	
	basic_string<T, _alloc_space>& assign_spgm(const T* s)
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return *this;
		for(_length = 0; _length != _alloc_space && *s != 0; ++s, ++_length)
			m_data[_length] = pgm_read_elem(s);
		return *this;
	}
	
// insert

	basic_string<T, _alloc_space>& insert_spgm(AVRLIB_STRING_SIZE_TYPE pos1, const T* s, AVRLIB_STRING_SIZE_TYPE n)
	{
		basic_string<T, _alloc_space> temp = substr(pos1);
		erase(pos1);
		append_spgm(s, n);
		append(temp);
		return *this;
	}
	
	basic_string<T, _alloc_space>& insert_spgm(AVRLIB_STRING_SIZE_TYPE pos1, const T* s)
	{
		basic_string<T, _alloc_space> temp = substr(pos1);
		erase(pos1);
		append_spgm(s);
		append(temp);
		return *this;
	}
	
// replace

	basic_string<T, _alloc_space>& replace_spgm ( AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n1,   const T* s, AVRLIB_STRING_SIZE_TYPE n2 )
	{
		erase(pos1, n1);
		insert_spgm(pos1, s, n2);
		return *this;
	}
	
	basic_string<T, _alloc_space>& replace_spgm ( T* i1, T* i2, const T* s, AVRLIB_STRING_SIZE_TYPE n2 )
	{
		erase(i1, i2 - 1);
		insert_spgm(i1 - begin(), s, n2);
		return *this;
	}
	
	basic_string<T, _alloc_space>& replace_spgm ( AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n1,   const T* s )
	{
		erase(pos1, n1);
		insert_spgm(pos1, s);
		return *this;
	}
	
	basic_string<T, _alloc_space>& replace_spgm ( T* i1, T* i2, const T* s )
	{
		erase(i1, i2 - 1);
		insert_spgm(i1 - begin(), s);
		return *this;
	}
	
// find

	AVRLIB_STRING_SIZE_TYPE find_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		if(pos >= _length || s == AVRLIB_STRING_NULLPTR || pgm_read_elem(0) == 0)
			return npos;
		for(;;)
		{
			for(; pos != _length; ++pos)
				if (m_data[pos] == pgm_read_elem(0))
					break;
			if (pos == _length)
				return npos;
			AVRLIB_STRING_SIZE_TYPE j = 1;
			T c = 0;
			for(; (pos + j) != _length && m_data[pos + j] == (c = pgm_read_elem(j)) && c != 0; ++j);
			if (c == 0)
				return pos;
			++pos;
		}
	}
	
	AVRLIB_STRING_SIZE_TYPE find_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		if(s == AVRLIB_STRING_NULLPTR || n == 0 || pos >= _length || n > _length || n > (_length - pos))
			return npos;
		for(;;)
		{
			for(; pos != _length; ++pos)
				if (m_data[pos] == pgm_read_elem(0))
					break;
			if (n > (_length - pos))
				return npos;
			for(AVRLIB_STRING_SIZE_TYPE j = 1; m_data[pos + j] == pgm_read_elem(j); ++j)
				if (j == n)
					return pos;
			++pos;
		}
	}
	
//rfind

	AVRLIB_STRING_SIZE_TYPE rfind_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE pos = npos) const
	{
		if(s == AVRLIB_STRING_NULLPTR || pgm_read_elem(0) == 0)
			return npos;
		AVRLIB_STRING_SIZE_TYPE len = strlen_spgm(s);
		if (len > _length)
			return npos;
		if (pos > (_length - len))
			pos = (_length - len);
		++pos;
		for(;;)
		{
			for(; pos != 0; --pos)
				if (m_data[pos - 1] == pgm_read_elem(0))
					break;
			if (pos == 0)
				return npos;
			for(AVRLIB_STRING_SIZE_TYPE j = 1; m_data[pos + j - 1] == pgm_read_elem(j); ++j)
				if (j == len)
					return pos - 1;
			--pos;
		}
	}
	
	AVRLIB_STRING_SIZE_TYPE rfind_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		if(s == AVRLIB_STRING_NULLPTR || pgm_read_elem(0) == 0)
			return npos;
		if (n > _length)
			return npos;
		if (pos > (_length - n))
			pos = (_length - n);
		++pos;
		for(;;)
		{
			for(; pos != 0; --pos)
				if (m_data[pos - 1] == pgm_read_elem(0))
					break;
			if (pos == 0)
				return npos;
			for(AVRLIB_STRING_SIZE_TYPE j = 1; m_data[pos + j - 1] == pgm_read_elem(j); ++j)
				if (j == n)
					return pos - 1;
			--pos;
		}
	}
	
// find_first_of

	AVRLIB_STRING_SIZE_TYPE find_first_of_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		if (s == AVRLIB_STRING_NULLPTR || n == 0)
			return npos;
		for(; pos < _length; ++pos)
			for(AVRLIB_STRING_SIZE_TYPE j = 0; j != n; ++j)
				if (m_data[pos] == pgm_read_elem(s + j))
					return pos;
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_first_of_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return npos;
		T c;
		for(; pos < _length; ++pos)
			for(AVRLIB_STRING_SIZE_TYPE j = 0; (c = pgm_read_elem(s + j)) != 0; ++j)
				if (m_data[pos] == c)
					return pos;
		return npos;
	}
	
//find_last_of

	AVRLIB_STRING_SIZE_TYPE find_last_of_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		if (s == AVRLIB_STRING_NULLPTR || n == 0)
			return npos;
		if (pos > _length)
			pos = _length;
		else
			++pos;
		for(; pos != 0; --pos)
			for(AVRLIB_STRING_SIZE_TYPE j = 0; j != n; ++j)
				if (m_data[pos - 1] == pgm_read_elem(s + j))
					return pos - 1;
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_last_of_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE pos = npos) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return npos;
		if (pos > _length)
			pos = _length;
		else
			++pos;
		T c;
		for(; pos != 0; --pos)
			for(AVRLIB_STRING_SIZE_TYPE j = 0; (c = pgm_read_elem(s + j)) != 0; ++j)
				if (m_data[pos - 1] == c)
					return pos - 1;
		return npos;
	}
	
//find_first_not_of
	
	AVRLIB_STRING_SIZE_TYPE find_first_not_of_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		if (s == AVRLIB_STRING_NULLPTR || n == 0)
			return npos;
		bool braked = false;
		for(; pos < _length; ++pos)
		{
			for(AVRLIB_STRING_SIZE_TYPE j = 0; j != n; ++j)
			{
				if (m_data[pos] == pgm_read_elem(s + j))
				{
					braked = true;
					break;
				}				
			}
			if (!braked)
				return pos;
			braked = false;
		}						
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_first_not_of_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return npos;
		bool braked = false;
		T c;
		for(; pos < _length; ++pos)
		{
			for(AVRLIB_STRING_SIZE_TYPE j = 0; (c = pgm_read_elem(s + j)) != 0; ++j)
			{
				if (m_data[pos] == c)
				{
					braked = true;
					break;
				}				
			}
			if (!braked)
				return pos;
			braked = false;
		}						
		return npos;
	}
	
//find_last_not_of

	AVRLIB_STRING_SIZE_TYPE find_last_not_of_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		if (s == AVRLIB_STRING_NULLPTR || n == 0)
			return npos;
		bool braked = false;
		if (pos > _length)
			pos = _length;
		else
			++pos;
		for(; pos != 0; --pos)
		{
			for(AVRLIB_STRING_SIZE_TYPE j = 0; j != n; ++j)
			{
				if (m_data[pos - 1] == pgm_read_elem(s + j))
				{
					braked = true;
					break;
				}				
			}
			if (!braked)
				return pos - 1;
			braked = false;
		}						
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_last_not_of_spgm(const T* s, AVRLIB_STRING_SIZE_TYPE pos = npos) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return npos;
		bool braked = false;
		if (pos > _length)
			pos = _length;
		else
			++pos;
		T c;
		for(; pos != 0; --pos)
		{
			for(AVRLIB_STRING_SIZE_TYPE j = 0; (c = pgm_read_elem(s + j)) != 0; ++j)
			{
				if (m_data[pos - 1] == c)
				{
					braked = true;
					break;
				}				
			}
			if (!braked)
				return pos - 1;
			braked = false;
		}						
		return npos;
	}

// compare

	int8_t compare_spgm ( const T* s ) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return empty() ? 0 : 1;
		AVRLIB_STRING_SIZE_TYPE i = 0;
		for(; i != _length; ++i)
		{
			T c = pgm_read_elem(s + i);
			if (c == 0)
				return 1;
			if (m_data[i] == c)
				continue;
			if (m_data[i] < c)
				return -1;
			if (m_data[i] > c)
				return 1;
		}
		if (pgm_read_elem(s + i) == 0)
			return 0;
		return -1;
	}
	
	int8_t compare_spgm ( AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n1, const T* s) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return empty() ? 0 : 1;
		n1 = (pos1 + n1) <= _length? n1 : (_length - pos1);
		AVRLIB_STRING_SIZE_TYPE i = 0;
		for(; i != n1; ++i)
		{
			T c = pgm_read_elem(s + i);
			if (*(s + i) == 0)
				return 1;
			if (m_data[i + pos1] == c)
				continue;
			if (m_data[i + pos1] < c)
				return -1;
			if (m_data[i + pos1] > c)
				return 1;
		}
		if (pgm_read_elem(s + i) == 0)
			return 0;
		return -1;
	}
	
	int8_t compare_spgm ( AVRLIB_STRING_SIZE_TYPE pos1, AVRLIB_STRING_SIZE_TYPE n1, const T* s, AVRLIB_STRING_SIZE_TYPE n2) const
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return empty() ? 0 : 1;
		n1 = (pos1 + n1) <= _length? n1 : (_length - pos1);
		if (n1 < n2)
			return -1;
		if (n1 > n2)
			return 1;
		for(AVRLIB_STRING_SIZE_TYPE i = 0; i != n1; ++i)
		{
			T c = pgm_read_elem(s + i);
			if (m_data[i + pos1] == c)
				continue;
			if (m_data[i + pos1] < c)
				return -1;
			if (m_data[i + pos1] > c)
				return 1;
		}
		return 0;
	}
	
// auxiliary static methods

	static AVRLIB_STRING_SIZE_TYPE strlen_spgm(const T* s)
	{
		if (s == AVRLIB_STRING_NULLPTR)
			return 0;
		AVRLIB_STRING_SIZE_TYPE n = 0;
		for (; pgm_read_elem(s) != 0; ++s)
			++n;
		return n;
	}
	
	static T pgm_read_elem(const T* const addr)
	{
		switch(sizeof(T))
		{
			case 1: return pgm_read_byte(addr);
			case 2: return pgm_read_word(addr);
			case 4: return pgm_read_dword(addr);
		}
		T res;
		memcpy_P(&res, addr, sizeof(T));
		return res;
	}
	
#endif // __PGMSPACE_H_
	
private:
	T m_data[_alloc_space];
	AVRLIB_STRING_SIZE_TYPE _length;	
	
};//end of class basic_string

typedef basic_string<char, AVRLIB_DEFAULT_STRING_SIZE> string; 

}//end of namespace


//comparison operators

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator == ( const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (lhs.compare(rhs) == 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator == ( const T* lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (rhs.compare(lhs) == 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator == ( const avrlib::basic_string<T, size>& lhs, const T* rhs )
{
	if (lhs.compare(rhs) == 0)
		return true;
	return false;
}


template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator != ( const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (lhs.compare(rhs) != 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator != ( const T* lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (rhs.compare(lhs) != 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator != ( const avrlib::basic_string<T, size>& lhs, const T* rhs )
{
	if (lhs.compare(rhs) != 0)
		return true;
	return false;
}


template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator < ( const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (lhs.compare(rhs) < 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator < ( const T* lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (rhs.compare(lhs) > 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator < ( const avrlib::basic_string<T, size>& lhs, const T* rhs )
{
	if (lhs.compare(rhs) < 0)
		return true;
	return false;
}


template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator > ( const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (lhs.compare(rhs) > 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator > ( const T* lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (rhs.compare(lhs) < 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator > ( const avrlib::basic_string<T, size>& lhs, const T* rhs )
{
	if (lhs.compare(rhs) > 0)
		return true;
	return false;
}


template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator <= ( const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (lhs.compare(rhs) <= 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator <= ( const T* lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (rhs.compare(lhs) >= 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator <= ( const avrlib::basic_string<T, size>& lhs, const T* rhs )
{
	if (lhs.compare(rhs) <= 0)
		return true;
	return false;
}


template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator >= ( const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (lhs.compare(rhs) >= 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator >= ( const T* lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (rhs.compare(lhs) <= 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator >= ( const avrlib::basic_string<T, size>& lhs, const T* rhs )
{
	if (lhs.compare(rhs) >= 0)
		return true;
	return false;
}


//operator +

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
avrlib::string operator + (const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs)
{
	avrlib::string ret = lhs;
	ret += rhs;
	return ret;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
avrlib::string operator + (const T* lhs, const avrlib::basic_string<T, size>& rhs)
{
	avrlib::string ret = lhs;
	ret += rhs;
	return ret;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
avrlib::string operator + (T lhs, const avrlib::basic_string<T, size>& rhs)
{
	avrlib::string ret;
	ret.assign(1, lhs);
	ret += rhs;
	return ret;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
avrlib::string operator + (const avrlib::basic_string<T, size>& lhs, const T* rhs)
{
	avrlib::string ret = lhs;
	ret += rhs;
	return ret;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
avrlib::string operator + (const avrlib::basic_string<T, size>& lhs, T rhs)
{
	avrlib::string ret = lhs;
	ret += rhs;
	return ret;
}


//swap

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
inline void swap ( avrlib::basic_string<T, size>& lhs, avrlib::basic_string<T, size>& rhs)
{
	lhs.swap(rhs);
}

#endif