#ifndef AVRLIB_STRING_HPP
#define AVRLIB_STRING_HPP

namespace avrlib {

#ifndef AVRLIB_DEFAULT_STRING_SIZE
#define AVRLIB_DEFAULT_STRING_SIZE 32
#endif

#ifndef AVRLIB_STRING_SIZE_TYPE 
#define AVRLIB_STRING_SIZE_TYPE uint8_t
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
	}//FIX ME
	
	basic_string<T, _alloc_space>& append(const T* s, AVRLIB_STRING_SIZE_TYPE n)
	{
		n = (_alloc_space - _length) < n? (_alloc_space - _length) : n; 
		for(AVRLIB_STRING_SIZE_TYPE i = 0; i != n; ++i, ++_length)
			m_data[_length] = *(s + i);
		return *this;
	}
	
	basic_string<T, _alloc_space>& append(const T* s)
	{
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
	
	
	inline void push_back(T c)
	{
		if (_length == _alloc_space)
			return;
		m_data[_length] = c;
		++_length;
	}
	
	
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
	
	basic_string<T, _alloc_space>& assign(const basic_string<T, _alloc_space>& str, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n)
	{
		return assign(str.substr(pos, n));
	}//FIX ME
	
	basic_string<T, _alloc_space>& assign(const T* s, AVRLIB_STRING_SIZE_TYPE n)
	{
		n = _alloc_space < n? _alloc_space : n;
		for (_length = 0; _length != n; ++_length, ++s)
			m_data[_length] = *s;
		return *this;
	}
	
	basic_string<T, _alloc_space>& assign(const T* s)
	{
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
		if ((pos + n) >= _length)
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
//NOT OPTIMALIZED!!!

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
//NOT OPTIMALIZED!!!

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
		n = n > (_length - pos) ? (_length - pos) : n;
		for(AVRLIB_STRING_SIZE_TYPE i = 0; i != n; ++i)
			*(s + i) = m_data[pos + i];
		return n;
	}
	
//find
	
	AVRLIB_STRING_SIZE_TYPE find(const basic_string<T, _alloc_space>& str,  AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
		if (str.empty() || str.length > _length || pos >= _length)
			return npos;
		for(;;)
		{
			for(; pos != _length; ++pos)
				if (m_data[pos] == str[0])
					break;
			if (pos == _length || (pos + str.length()) > _length)
				return npos;
			for(AVRLIB_STRING_SIZE_TYPE j = 0; m_data[pos + j] == str[j]; ++j)
				if (j == str.length())
					return pos;
			++pos;
		}
	}
	
	//function AVRLIB_STRING_SIZE_TYPE find(const T* s, AVRLIB_STRING_SIZE_TYPE pos) const
	//is implemented by function above, because constructor for C_string exist
	
	AVRLIB_STRING_SIZE_TYPE find(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		const basic_string<T, _alloc_space> temp (s, n);
		return find(temp, pos);
	}//FIX ME
	
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
			for(AVRLIB_STRING_SIZE_TYPE j = 0; m_data[pos + j - 1] == str[j]; ++j)
				if (j == str.length())
					return pos - 1;
			--pos;
		}
	}
	
	//function AVRLIB_STRING_SIZE_TYPE rfind(const T* s, AVRLIB_STRING_SIZE_TYPE pos = npos) const
	//is implemented by function above, because constructor for C_string exist
	
	AVRLIB_STRING_SIZE_TYPE rfind(const T* s, AVRLIB_STRING_SIZE_TYPE pos, AVRLIB_STRING_SIZE_TYPE n) const
	{
		const basic_string<T, _alloc_space> temp (s, n);
		return rfind(temp, pos);
	}//FIX ME
	
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
		if (n == 0)
			return npos;
		for(; pos < _length; ++pos)
			for(AVRLIB_STRING_SIZE_TYPE j = 0; j != n; ++j)
				if (m_data[pos] == *(s + j))
					return pos;
		return npos;
	}
	
	AVRLIB_STRING_SIZE_TYPE find_first_of(const T* s, AVRLIB_STRING_SIZE_TYPE pos = 0) const
	{
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
		if (n == 0)
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
		if (n == 0)
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
		if (n == 0)
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
	
private:
	char m_data[_alloc_space];
	AVRLIB_STRING_SIZE_TYPE _length;	
	
};//end of class basic_string

typedef basic_string<char, AVRLIB_DEFAULT_STRING_SIZE> string; 

}//end of namespace


//comparison operators

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator== ( const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (lhs.compare(rhs) == 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator== ( const char* lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (rhs.compare(lhs) == 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator== ( const avrlib::basic_string<T, size>& lhs, const char* rhs )
{
	if (lhs.compare(rhs) == 0)
		return true;
	return false;
}


template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator!= ( const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (lhs.compare(rhs) != 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator!= ( const char* lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (rhs.compare(lhs) != 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator!= ( const avrlib::basic_string<T, size>& lhs, const char* rhs )
{
	if (lhs.compare(rhs) != 0)
		return true;
	return false;
}


template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator< ( const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (lhs.compare(rhs) < 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator< ( const char* lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (rhs.compare(lhs) > 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator< ( const avrlib::basic_string<T, size>& lhs, const char* rhs )
{
	if (lhs.compare(rhs) < 0)
		return true;
	return false;
}


template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator> ( const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (lhs.compare(rhs) > 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator> ( const char* lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (rhs.compare(lhs) < 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator> ( const avrlib::basic_string<T, size>& lhs, const char* rhs )
{
	if (lhs.compare(rhs) > 0)
		return true;
	return false;
}


template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator<= ( const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (lhs.compare(rhs) <= 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator<= ( const char* lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (rhs.compare(lhs) >= 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator<= ( const avrlib::basic_string<T, size>& lhs, const char* rhs )
{
	if (lhs.compare(rhs) <= 0)
		return true;
	return false;
}


template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator>= ( const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (lhs.compare(rhs) >= 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator>= ( const char* lhs, const avrlib::basic_string<T, size>& rhs )
{
	if (rhs.compare(lhs) <= 0)
		return true;
	return false;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
bool operator>= ( const avrlib::basic_string<T, size>& lhs, const char* rhs )
{
	if (lhs.compare(rhs) >= 0)
		return true;
	return false;
}


//operator +

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
avrlib::string operator+ (const avrlib::basic_string<T, size>& lhs, const avrlib::basic_string<T, size>& rhs)
{
	avrlib::string ret = lhs;
	ret += rhs;
	return ret;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
avrlib::string operator+ (const char* lhs, const avrlib::basic_string<T, size>& rhs)
{
	avrlib::string ret = lhs;
	ret += rhs;
	return ret;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
avrlib::string operator+ (char lhs, const avrlib::basic_string<T, size>& rhs)
{
	avrlib::string ret;
	ret.assign(1, lhs);
	ret += rhs;
	return ret;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
avrlib::string operator+ (const avrlib::basic_string<T, size>& lhs, const char* rhs)
{
	avrlib::string ret = lhs;
	ret += rhs;
	return ret;
}

template <typename T, AVRLIB_STRING_SIZE_TYPE size>
avrlib::string operator+ (const avrlib::basic_string<T, size>& lhs, char rhs)
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