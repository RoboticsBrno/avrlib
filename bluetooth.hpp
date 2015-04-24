#ifndef AVRLIB_BLUETOOTH_HPP
#define AVRLIB_BLUETOOTH_HPP

/*
 * WARNING: 
 * For Atmel Studio 6 and higher you must deactivated "Pack Structure members together (-fpack-struct)" in Properties -> Toolchain -> Optimization (for right compiler - C Compiler/C++ Compiler) 
 *
 * Else you couldn't compile project and you will have this error: "Warning 1 ignoring packed attribute because of unpacked non-POD field 'avrlib::bt_t<avrlib::async_usart<avrlib::uart_xmega, 64, 64> >::usart_type& avrlib::bt_t<avrlib::async_usart<avrlib::uart_xmega, 64, 64> >::m_usart' [enabled by default]" 
 */

namespace avrlib {
	
volatile uint8_t addr[][26] =
{
{0x00, 0x12, 0xf3, 0x07, 0xbf, 0xf3},//0 - newBT0
{0x00, 0x12, 0xf3, 0x07, 0xc0, 0x32},//1 - Yunitooth_new_1
{0x00, 0x12, 0xf3, 0x07, 0xc0, 0x26},//2 - Yunitooth_new_3 = Chessbot
{0x00, 0x12, 0xf3, 0x07, 0xc0, 0x1f},//3 - yunitooth05
{0x00, 0x12, 0xf3, 0x07, 0xc0, 0x35},//4 - yunitooth_new_50
{0x00, 0x12, 0xf3, 0x04, 0xcd, 0x28},//5 - yunitooth_long_0
{0x00, 0x12, 0xf3, 0x04, 0xa9, 0xeb},//6 - kubuv_robot
{0x00, 0x12, 0xf3, 0x04, 0xa9, 0xfa},//7 - SpiderTooth
{0x00, 0x12, 0xf3, 0x07, 0xbf, 0xf4},//8 - Yunitooth_new_2
{0x00, 0x12, 0xf3, 0x04, 0xcd, 0x80},//9 - Aranka_motors_debug (yunitooth_long_1)
{0x00, 0x12, 0xf3, 0x04, 0xaa, 0x06},//10 - Avakar's Yunimin
{0x00, 0x12, 0xf3, 0x0e, 0xb3, 0x76},//11 - gimly = Enkeli (Vojta Urban)
{0x00, 0x12, 0xf3, 0x18, 0x2a, 0x3b},//12 - Yunibeer = Alwood (Franta Èaslavský)
{0x00, 0x12, 0xf3, 0x15, 0xb7, 0x3b},//13 - Cylon_control	
{0x00, 0x12, 0xf3, 0x15, 0xb7, 0x2c},//14 - Cylon2_tooth	
{0x00, 0x12, 0xf3, 0x18, 0x2a, 0x41},//15 - Yunibeer_transmitter	
{0x20, 0x13, 0x08, 0x09, 0x10, 0x04},//16 - 3pi_JarekP	
{0x20, 0x13, 0x08, 0x09, 0x09, 0x95},//17 - 3pi_Sok_01	
{0x20, 0x13, 0x08, 0x09, 0x08, 0x49},//18 - 3pi_Sok_02
{0x20, 0x13, 0x08, 0x09, 0x07, 0x60},//19 - 3pi_Sok_03	
{0x20, 0x13, 0x08, 0x09, 0x10, 0x23},//20 - 3pi_Sok_04
{0x20, 0x14, 0x02, 0x17, 0x10, 0x23},//21 - 3pi_Sok_05
{0x20, 0x14, 0x02, 0x17, 0x11, 0x53},//22 - 3pi_Sok_06
{0x20, 0x13, 0x08, 0x09, 0x08, 0x23},//23 - 3pi_Sok_07
{0x20, 0x14, 0x02, 0x17, 0x11, 0x52},//24 - 3pi_Sok_09
{0x20, 0x14, 0x02, 0x17, 0x11, 0x48} //25 - 3pi_Sok_10
};	
	
template <typename Usart>
class bt_t
{
	public:
	typedef Usart usart_type;
	
	bt_t(usart_type &s)
		:m_usart(s), m_connected(false)
	{}
	
	bool connect(uint8_t a)
	{
		m_usart.flush();
		
		_delay_ms(1100);
		
		send(m_usart, "///");
		
		m_usart.flush();
		
		_delay_ms(1100);
		
		send(m_usart, "AT*ADNRP=1,0\r");	//No Of Remote Peers
		m_usart.flush();
		send(m_usart, "AT*ADDCP=0,0\r");	//Default Client Profile = Serial Port Profile
		m_usart.flush();
		send(m_usart, "AT*ADWDRP=0,");	//Write Default Remote Peer
		m_usart.flush();
		
		for (int i = 0; i < 6; ++i)
		{
			char const digits[] = "0123456789abcdef";

			m_usart.write(digits[addr[a][i] >> 4]);
			m_usart.write(digits[addr[a][i] & 0x0f]);
			m_usart.flush();
		}

		send(m_usart, ",2,0,\"\",0\r");
		m_usart.flush();
		send(m_usart, "AT*ADDM\r");	//Data Mode
		m_usart.flush();
		
		m_connected = true;
		return true;
	}
	
	bool disconnect()
	{
		if(!m_connected)
			return true;
		_delay_ms(1100);
		send(m_usart, "///");
		m_usart.flush();
		_delay_ms(1100);
		send(m_usart, "AT*ADNRP=0,0\r");
		m_usart.flush();
		send(m_usart, "AT*ADDM\r");
		m_usart.flush();
		m_connected = false;
		return true;
	}
	
	bool search(uint8_t inquiry_type = 1, uint8_t inquiry_length = 24)
	{
		_delay_ms(1100);

		send(m_usart, "///");	
		m_usart.flush();
		
		_delay_ms(1100);
		
		send(m_usart, "AT*AGDD=2,4\r");
		m_usart.flush();
		send(m_usart, "AT*ADDM\r");		
		m_usart.flush();
		
		return true;
	}
	
	bool inquiry(uint8_t inquiry_type = 1, uint8_t inquiry_length = 24)
	{
		_delay_ms(1100);

		send(m_usart, "///");
		m_usart.flush();
		
		_delay_ms(1100);
		
		send(m_usart, "AT*AGI=2,4,0\r");
		m_usart.flush();
		send(m_usart, "AT*ADDM\r");		
		m_usart.flush();
		
		return true;
	}
	
	bool connected() { return m_connected; }
	
private:
	usart_type &m_usart;
	bool m_connected;
};
	
}

#endif

// Example - How setting connection in the code:

// using avrlib::bt_t;

// avrlib::async_usart <usart_xd0, 64, 64> us_xd0(51);
// bt_t<avrlib::async_usart<usart_xd0, 64, 64> > bt_xd0(us_xd0);

// bt_xd0.connect(2);

// It is everything.