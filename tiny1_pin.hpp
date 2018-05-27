#ifndef AVRLIB_TINY1_PIN_HPP
#define AVRLIB_TINY1_PIN_HPP

#include <avr/io.h>
#include <stdint.h>

#define AVRLIB_DEFINE_TINY1_PIN(pin_name, port, pin) \
	struct pin_name \
	{ \
		static uint8_t const bp = pin; \
		static uint8_t const bm = (1<<pin); \
		static PORT_t& PORT() { return port; } \
		static void make_input() { port##_DIRCLR = (1<<(pin)); } \
		static void make_high() { port##_OUTSET = (1<<(pin)); port##_DIRSET = (1<<(pin)); } \
		static void make_low() { port##_OUTCLR = (1<<(pin)); port##_DIRSET = (1<<(pin)); } \
		static void make_output() { port##_DIRSET = (1<<(pin)); } \
		static void set_value(uint8_t value) { if (value) port##_OUTSET = (1<<(pin)); else port##_OUTCLR = (1<<(pin)); } \
		static void set_high() { port##_OUTSET = (1<<(pin)); } \
		static void set_low() { port##_OUTCLR = (1<<(pin)); } \
		static uint8_t get_value() { return (port##_OUT & (1<<(pin))) != 0; } \
		static uint8_t get_dir() { return (port##_DIR & (1<<(pin))) != 0; } \
		static void toggle() { port##_OUTTGL = (1<<(pin)); } \
		static void toggle_dir() { port##_DIRTGL = (1<<(pin)); } \
		static void pullup() { port##_PIN##pin##CTRL |= PORT_PULLUPEN_bm; } \
		static bool read() { return (port##_IN & (1<<(pin))) != 0; } \
		static void pinctrl(uint8_t v) { port##_PIN##pin##CTRL = v; } \
		static uint8_t pinctrl() { return port##_PIN##pin##CTRL; } \
		static void make_inverted() { port##_PIN##pin##CTRL |= PORT_INVEN_bm; } \
		static void make_noninverted() { port##_PIN##pin##CTRL &= ~PORT_INVEN_bm; } \
		static void sence_both_edges() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_ISC_gm) | PORT_ISC_BOTHEDGES_gc; } \
		static void sence_rising_edge() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_ISC_gm) | PORT_ISC_RISING_gc; } \
		static void sence_faling_edge() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_ISC_gm) | PORT_ISC_FALLING_gc; } \
		static void sence_level() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_ISC_gm) | PORT_ISC_LEVEL_gc; } \
		static void input_disable() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_ISC_gm) | PORT_ISC_INPUT_DISABLE_gc; } \
		static void intr_disable() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_ISC_gm) | PORT_ISC_INTDISABLE_gc; } \
	}

namespace avrlib {

template <typename PinHigh, typename PinLow>
struct pin_totem_drive
{
	static void make_input() { PinHigh::set_low(); PinLow::set_low(); }
	static void make_high() { PinLow::set_low(); PinHigh::set_high(); }
	static void make_low() { PinHigh::set_low(); PinLow::set_high(); }
	static void set_value(uint8_t value) { if (value) set_high(); else set_low(); }
	static void set_high() { PinLow::set_low(); PinHigh::set_high(); }
	static void set_low() { PinHigh::set_low(); PinLow::set_high(); }
};

template <typename ValuePin, typename OePin>
struct pin_buffer_with_oe
{
	typedef ValuePin value_pin;
	typedef OePin oe_pin;

	static void init(const bool& invert_oe = false)
	{
		ValuePin::make_input();
		if(invert_oe)
			OePin::make_inverted();
		OePin::make_low();
	}

	static void clear()
	{
		ValuePin::make_input();
		OePin::make_input();
	}

	static void make_input()
	{
		ValuePin::make_input();
		OePin::set_low();
	}

	static void make_high()
	{
		OePin::set_high();
		ValuePin::make_high();
	}

	static void make_low()
	{
		OePin::set_high();
		ValuePin::make_low();
	}

	static void make_output()
	{
		OePin::set_high();
		ValuePin::make_output();
	}

	static void set_value(uint8_t value)
	{
		ValuePin::set_value(value);
	}

	static void set_high()
	{
		ValuePin::set_high();
	}

	static void set_low()
	{
		ValuePin::set_low();
	}

	static bool read()
	{
		return ValuePin::read();
	}

	static void toggle()
	{
		ValuePin::toggle();
	}
	
	static void toggle_dir()
	{
		OePin::toggle();
	}

	static void make_inverted()
	{
		ValuePin::make_inverted();
	}

	static void make_noninverted()
	{
		ValuePin::make_noninverted();
	}
};

struct pin_t
{
	pin_t(PORT_t& port, const uint8_t& pin)
		:port(port), bp(pin), bm(1<<pin)
	{}
	pin_t(const pin_t& pin)
		:port(pin.port), bp(pin.bp), bm(pin.bm)
	{}
	void make_input() { port.DIRCLR = bm; }
	void make_high() { port.OUTSET = bm; port.DIRSET = bm; }
	void make_low() { port.OUTCLR = bm; port.DIRSET = bm; }
	void make_output() { port.DIRSET = bm; }
	void set_value(uint8_t value) { if (value) port.OUTSET = bm; else port.OUTCLR = bm; }
	void set_high() { port.OUTSET = bm; }
	void set_low() { port.OUTCLR = bm; }
	uint8_t get_value() const { return (port.OUT & bm) != 0; }
	uint8_t get_dir() const { return (port.DIR & bm) != 0; }
	void toggle() { port.OUTTGL = bm; }
	void toggle_dir() { port.DIRTGL = bm; }
	void pullup() { *(&port.PIN0CTRL + bp) |= PORT_PULLUPEN_bm; }
	bool read() const { return (port.IN & bm) != 0; }
	void pinctrl(uint8_t v) { *(&port.PIN0CTRL + bp) = v; }
	uint8_t pinctrl() const { return *(&port.PIN0CTRL + bp); }
	void make_inverted() { *(&port.PIN0CTRL + bp) |= PORT_INVEN_bm; }
	void make_noninverted() { *(&port.PIN0CTRL + bp) &= ~PORT_INVEN_bm; }
	bool is_inverted() const { return (*(&port.PIN0CTRL + bp) & PORT_INVEN_bm) != 0; }
	void sence_both_edges() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_ISC_gm) | PORT_ISC_BOTHEDGES_gc; }
	void sence_rising_edge() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_ISC_gm) | PORT_ISC_RISING_gc; }
	void sence_faling_edge() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_ISC_gm) | PORT_ISC_FALLING_gc; }
	void sence_level() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_ISC_gm) | PORT_ISC_LEVEL_gc; }
	void input_disable() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_ISC_gm) | PORT_ISC_INPUT_DISABLE_gc; }
	void intr_disable() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_ISC_gm) | PORT_ISC_INTDISABLE_gc; }
	uint8_t is_intr_enabled() const { return (*(&port.PIN0CTRL + bp) & PORT_ISC_gm) != PORT_ISC_INTDISABLE_gc; }

	PORT_t& port;
	uint8_t const bp;
	uint8_t const bm;
};

} // end of namespace avrlib

#endif // AVRLIB_TINY1_PIN_HPP
