#ifndef AVRLIB_INTR_PRIO_HPP
#define AVRLIB_INTR_PRIO_HPP

namespace avrlib {

enum intr_prio_t
{
	intr_disabled = 0,
	intr_enabled = 1,
	intr_lo = 1,
	intr_med = 2,
	intr_hi = 3,
};

}

#endif
