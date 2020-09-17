#include "TickAnimation.h"


TickAnimation* TickAnimation::clone(animid_t newId) const {
	return new TickAnimation(delay, repetitions, newId);
}

void TickAnimation::executeTickFunc() {
	tickFunc();
}

delay_t TickAnimation::getDelay() {
	return delay;
}

int TickAnimation::getRepetitions() {
	return repetitions;
}