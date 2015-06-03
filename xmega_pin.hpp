#ifndef AVRLIB_XMEGA_PIN_HPP
#define AVRLIB_XMEGA_PIN_HPP

#include <avr/io.h>
#include <stdint.h>

#define AVRLIB_DEFINE_XMEGA_PIN(pin_name, port, pin) \
	struct pin_name \
	{ \
		static uint8_t const bp = pin; \
		static uint8_t const bm = (1<<pin); \
		static uint8_t const int_vect_0 = port##_INT0_vect_num; \
		static uint8_t const int_vect_1 = port##_INT1_vect_num; \
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
		static void pullup() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_OPC_gm) | PORT_OPC_PULLUP_gc; } \
		static void pulldown() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_OPC_gm) | PORT_OPC_PULLDOWN_gc; } \
		static void wiredor() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_OPC_gm) | PORT_OPC_WIREDOR_gc; } \
		static void wiredand() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_OPC_gm) | PORT_OPC_WIREDAND_gc; } \
		static void wiredor_pull() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_OPC_gm) | PORT_OPC_WIREDORPULL_gc; } \
		static void wiredand_pull() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_OPC_gm) | PORT_OPC_WIREDANDPULL_gc; } \
		static void totem() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_OPC_gm) | PORT_OPC_TOTEM_gc; } \
		static void buskeeper() { port##_PIN##pin##CTRL = (port##_PIN##pin##CTRL & ~PORT_OPC_gm) | PORT_OPC_BUSKEEPER_gc; } \
		static bool read() { return (port##_IN & (1<<(pin))) != 0; } \
		static void pinctrl(uint8_t v) { port##_PIN##pin##CTRL = v; } \
		static void make_inverted() { port##_PIN##pin##CTRL |= PORT_INVEN_bm; } \
		static void make_noninverted() { port##_PIN##pin##CTRL &= ~PORT_INVEN_bm; } \
		static void slew_rate_limit_enable() { port##_PIN##pin##CTRL |= PORT_SRLEN_bm; } \
		static void slew_rate_limit_disable() { port##_PIN##pin##CTRL &= ~PORT_SRLEN_bm; } \
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
	void pullup() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_OPC_gm) | PORT_OPC_PULLUP_gc; }
	void pulldown() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_OPC_gm) | PORT_OPC_PULLDOWN_gc; }
	void wiredor() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_OPC_gm) | PORT_OPC_WIREDOR_gc; }
	void wiredand() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_OPC_gm) | PORT_OPC_WIREDAND_gc; }
	void wiredor_pull() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_OPC_gm) | PORT_OPC_WIREDORPULL_gc; }
	void wiredand_pull() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_OPC_gm) | PORT_OPC_WIREDANDPULL_gc; }
	void totem() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_OPC_gm) | PORT_OPC_TOTEM_gc; }
	void buskeeper() { *(&port.PIN0CTRL + bp) = (*(&port.PIN0CTRL + bp) & ~PORT_OPC_gm) | PORT_OPC_BUSKEEPER_gc; }
	bool read() const { return (port.IN & bm) != 0; }
	void pinctrl(uint8_t v) { *(&port.PIN0CTRL + bp) = v; }
	void make_inverted() { *(&port.PIN0CTRL + bp) |= PORT_INVEN_bm; }
	void make_noninverted() { *(&port.PIN0CTRL + bp) &= ~PORT_INVEN_bm; }
	void slew_rate_limit_enable() { *(&port.PIN0CTRL + bp) |= PORT_SRLEN_bm; }
	void slew_rate_limit_disable() { *(&port.PIN0CTRL + bp) &= ~PORT_SRLEN_bm; }
	void intr0_en(uint8_t v) { if(v) port.INT0MASK |= bm; else port.INT0MASK &= ~bm; }
	void intr1_en(uint8_t v) { if(v) port.INT1MASK |= bm; else port.INT1MASK &= ~bm; }
	void intr_en(uint8_t n, uint8_t en)
	{
		switch(n)
		{
			case 0: intr0_en(en); break;
			case 1: intr1_en(en); break;
		}
	}
	
	PORT_t& port;
	uint8_t const bp;
	uint8_t const bm;
};

#endif // AVRLIB_XMEGA_PIN_HPP
