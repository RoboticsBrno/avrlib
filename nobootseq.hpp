#ifndef AVRLIB_NOBOOTSEQ_HPP
#define AVRLIB_NOBOOTSEQ_HPP

#include <stdlib.h>

namespace avrlib {

class nobootseq
{
public:
	uint8_t check(uint8_t v)
	{
		return v;
	}
};

}

#endif
