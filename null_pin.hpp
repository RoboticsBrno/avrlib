#ifndef AVRLIB_NULL_PIN_HPP
#define AVRLIB_NULL_PIN_HPP

namespace avrlib {

template <bool Value>
struct null_pin
{
	static void set(bool value = true) {}
	static void clear() {}
	static bool get() { return Value; }
	static void output(bool value) {}
	static void make_input() {}
	static void make_low() {}
	static void make_high() {}
	static void set_value(bool value) {}
	static void set_high() {}
	static void set_low() {}
};

}

#endif
