#ifndef AVRLIB_BLUETOOTH_HPP
#define AVRLIB_BLUETOOTH_HPP

namespace avrlib {
	
volatile uint8_t addr[][11] =
{
{0x00, 0x12, 0xf3, 0x07, 0xbf, 0xf3},//0 - newBT0
{0x00, 0x12, 0xf3, 0x07, 0xc0, 0x32},//1 - Yunitooth_new_1
{0x00, 0x12, 0xf3, 0x07, 0xc0, 0x26},//2 - Yunitooth_new_3 = Chessbot
{0x00, 0x12, 0xf3, 0x07, 0xc0, 0x1f},//3 - yunitooth05
{0x00, 0x12, 0xf3, 0x07, 0xc0, 0x35},//4 - yunitooth_new_50
{0x00, 0x12, 0xf3, 0x04, 0xcd, 0x28},//5 - yunitooth_long_0
{0x00, 0x12, 0xf3, 0x04, 0xa9, 0xeb},//6 - kubuv robot
{0x00, 0x12, 0xf3, 0x04, 0xa9, 0xfa},//7 - Spaintooth
{0x00, 0x12, 0xf3, 0x07, 0xbf, 0xf4},//8 - Yunitooth_new_2
{0x00, 0x12, 0xf3, 0x04, 0xcd, 0x80},//9 - yunitooth_long_1
{0x00, 0x12, 0xf3, 0x04, 0xaa, 0x06} //10 - Avakar's Yunimin

};	
	
template <typename Usart>
class bt_t
{
	public:
	typedef Usart usart_type;
	
	bt_t(usart_type &s)
	:m_usart(s)
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
	
	private:
	usart_type &m_usart;
};
	
}

#endif

// Example - How setting connection in the code:

// using avrlib::bt_t;

// avrlib::async_usart <usart_xd0, 64, 64> us_xd0(51);
// bt_t<avrlib::async_usart<usart_xd0, 64, 64> > bt_xd0(us_xd0);

// bt_xd0.connect(2);

// It is everything.