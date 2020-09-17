#pragma once

#include "Animation.h"
#include <functional>

class TickAnimation : public Animation {
private:
	delay_t delay;
	int repetitions;
	std::function<void(void)> tickFunc;
public:
	TickAnimation(delay_t delay, int reps, animid_t id) : 
		Animation(id), delay(delay), repetitions(reps) {
		tickFunc = nullptr; 
	}
	template <typename Tfunc>
	void setTickFunc(const Tfunc& tickFunction) {
		tickFunc = tickFunction;
	}
	virtual TickAnimation* clone(animid_t newId) const override;
	void executeTickFunc();
	delay_t getDelay();
	int getRepetitions();
};