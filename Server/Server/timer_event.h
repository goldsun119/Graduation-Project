#include <chrono>
#include <queue>
#include "Player.h"

struct T_EVENT {
	std::chrono::high_resolution_clock::time_point start_time;
	int			do_object;
	EVENT_TYPE	event_type;

	constexpr bool operator < (const T_EVENT& _Left) const
	{	// apply operator< to operands
		return (start_time > _Left.start_time);
	}
};
class timer_event
{
public:
	timer_event();
	~timer_event();
};

