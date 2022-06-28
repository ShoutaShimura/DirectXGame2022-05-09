#include "TimedCall.h"

TimedCall::TimedCall(std::function<int(int)> f, uint32_t time)
	:f(f),time_(time)
{
}

TimedCall::~TimedCall()
{
}

void TimedCall::Update()
{
	if (completion) {
		return;
	}
	time_--;

	if (time_ <= 0) {
		completion = true;

		f();
	}
}
