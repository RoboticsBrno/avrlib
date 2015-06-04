#ifndef AVRLIB_PIN_HPP
#define AVRLIB_PIN_HPP

namespace avrlib {

template <typename Port, int Pin>
struct pin
{
	static void set(bool value = true)
	{
		if (value)
			Port::port(Port::port() | (1<<Pin));
		else
			Port::port(Port::port() & ~(1<<Pin));
	}

	static void clear() { set(false); }
		
	static void toggle() { Port::port(Port::port() ^ (1<<Pin)); }

	static bool get() { return (Port::port() & (1<<Pin)) != 0; }

	static bool value() { return (Port::pin() & (1<<Pin)) != 0; }

	static void output(bool value)
	{
		if (value)
			Port::dir(Port::dir() | (1<<Pin));
		else
			Port::dir(Port::dir() & ~(1<<Pin));
	}

	static bool output() { return (Port::dir() & (1<<Pin)) != 0; }

	static void make_output() { output(true); }
	static void make_input() { output(false); clear(); }
	static void make_low() { clear(); output(true); }
	static void make_high() { set(); output(true); }
	static void set_value(bool value) { set(value); }
	static void set_high() { set(true); }
	static void set_low() { set(false); }
	static bool read() { return value(); }
	static void pullup() { set_high(); }
};

template <typename Port, int Pin>
struct inv_pin
{
	static void set(bool value = true)
	{
		if (value)
			Port::port(Port::port() & ~(1<<Pin));
		else
			Port::port(Port::port() | (1<<Pin));
	}

	static void clear() { set(false); }
	
	static void toggle() { Port::port(Port::port() ^ (1<<Pin)); }

	static bool get() { return (Port::port() & (1<<Pin)) == 0; }

	static bool value() { return (Port::pin() & (1<<Pin)) == 0; }

	static void output(bool value)
	{
		if (value)
			Port::dir(Port::dir() | (1<<Pin));
		else
			Port::dir(Port::dir() & ~(1<<Pin));
	}

	static bool output() { return (Port::dir() & (1<<Pin)) != 0; }

	static void make_output() { output(true); }
	static void make_input() { output(false); clear(); }
	static void make_low() { clear(); output(true); }
	static void make_high() { set(); output(true); }
	static void set_value(bool value) { set(value); }
	static void set_high() { set(true); }
	static void set_low() { set(false); }
	static bool read() { return value(); }
	static void pullup() { set_low(); }
};

template <bool Value>
struct dummy_pin
{
	static void set(bool) {}

	static void clear() {}
		
	static void toggle() {}

	static bool get() { return Value; }

	static bool value() { return Value; }

	static void output(bool) {}

	static bool output() { return false; }

	static void make_output() {}
	static void make_input() {}
	static void make_low() {}
	static void make_high() {}
	static void set_value(bool) {}
	static void set_high() {}
	static void set_low() {}
	static bool read() { return Value; }
	static void pullup() {}
};

}

#endif
