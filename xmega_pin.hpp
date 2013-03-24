#ifndef AVRLIB_XMEGA_PIN_HPP
#define AVRLIB_XMEGA_PIN_HPP

#include <avr/io.h>
#include <stdint.h>

#define AVRLIB_DEFINE_XMEGA_PIN(pin_name, port, pin) \
	struct pin_name \
	{ \
		static uint8_t const bp = pin; \
		static uint8_t const bm = (1<<pin); \
		static void make_input() { port##_DIRCLR = (1<<(pin)); } \
		static void make_high() { port##_OUTSET = (1<<(pin)); port##_DIRSET = (1<<(pin)); } \
		static void make_low() { port##_OUTCLR = (1<<(pin)); port##_DIRSET = (1<<(pin)); } \
		static void make_output() { port##_DIRSET = (1<<(pin)); } \
		static void set_value(uint8_t value) { if (value) port##_OUTSET = (1<<(pin)); else port##_OUTCLR = (1<<(pin)); } \
		static void set_high() { port##_OUTSET = (1<<(pin)); } \
		static void set_low() { port##_OUTCLR = (1<<(pin)); } \
		static uint8_t get_value() { return (port##_OUT & (1<<(pin))) != 0; } \
		static void toggle() { port##_OUTTGL = (1<<(pin)); } \
		static void pullup() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_OPC_gm) | PORT_OPC_PULLUP_gc; } \
		static void pulldown() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_OPC_gm) | PORT_OPC_PULLDOWN_gc; } \
		static void totem() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_OPC_gm) | PORT_OPC_TOTEM_gc; } \
		static void buskeeper() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_OPC_gm) | PORT_OPC_BUSKEEPER_gc; } \
		static bool read() { return (port##_IN & (1<<(pin))) != 0; } \
		static void pinctrl(uint8_t v) { port##_PIN##pin##CTRL = v; } \
		static void make_inverted() { port##_PIN##pin##CTRL |= PORT_INVEN_bm; } \
		static void make_noninverted() { port##_PIN##pin##CTRL &= ~PORT_INVEN_bm; } \
	}

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

	static void init()
	{
		ValuePin::make_input();
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

	static void make_inverted()
	{
		ValuePin::make_inverted();
	}

	static void make_noninverted()
	{
		ValuePin::make_noninverted();
	}
};


#endif // AVRLIB_XMEGA_PIN_HPP
