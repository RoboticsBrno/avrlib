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
};

}

#endif
