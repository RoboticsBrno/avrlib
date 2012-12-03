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
		static bool read() { return (port##_IN & (1<<(pin))) != 0; } \
		static void pinctrl(uint8_t v) { port##_PIN##pin##CTRL = v; } \
	}

#endif // AVRLIB_XMEGA_PIN_HPP
