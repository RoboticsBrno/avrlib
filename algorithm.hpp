#ifndef AVRLIB_ALGORITHM_HPP
#define AVRLIB_ALGORITHM_HPP

namespace avrlib
{

template <typename T>
void swap(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
}
template <typename T>
void swap(T *a, T *b, uint16_t length)
{
	T temp;
	for(uint16_t i = 0; i != length; ++i)
	{
		temp = *(a + i);
		*(a + i) = *(b + i);
		*(b + i) = temp;
	}
}

template <typename T1, typename T2>
uint16_t find(const T1& what, const T2* where, uint16_t length)
{
	for(uint16_t i = 0; i != length; ++i)
		if (what == *(where + i))
			return i;
	return -1;
}

template <typename data_type, typename ret_type>
ret_type sum(const data_type *data, uint8_t length)
{
	ret_type temp = 0;
	for(; length-- != 0;)
		temp += data[length];
	return temp;
}

template <typename data_type, typename sum_type, typename ret_type>
ret_type average(const data_type *data, uint8_t length)
{
	return sum<data_type, sum_type>(data, length) / ret_type(length);
}

template <typename T>
T min(const T *data, uint8_t length)
{
	T temp = data[--length];
	for(; length-- != 0;)
		if(temp > data[length])
			temp = data[length];
	return temp;
}

template <typename T>
T min(const T &a, const T &b)
{
	if(a < b)
		return a;
	return b;
}

template <typename T>
T max(const T *data, uint8_t length)
{
	T temp = data[--length];
	for(; length-- != 0;)
		if(temp < data[length])
			temp = data[length];
	return temp;
}

template <typename T>
T max(const T &a, const T &b)
{
	if(a > b)
		return a;
	return b;
}

template <typename T>
void extrems(const T *data, uint8_t length, T &min, T &max)
{
	min = data[--length];
	max = data[length];
	for(; length-- != 0;)
	{
		if(min > data[length])
			min = data[length];
		if(max < data[length])
			max = data[length];
	}	
}

} // end of namespace avrlib

#endif