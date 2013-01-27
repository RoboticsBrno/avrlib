#include "usart0.hpp"
#include "usart1.hpp"
#include "sync_usart.hpp"
#include "format.hpp"
using namespace avrlib;

sync_usart<usart1> rs232(38400);

int main()
{
	send(rs232, "Ahoj\r\n");
}
