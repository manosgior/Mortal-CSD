#include "Animator.h"
#include "AnimatorHolder.h"


void registerAtAnimatorHolder(Animator *animator) {
	AnimatorHolder::Register(animator);
}

void (*Animator::registerAtHolder)(Animator*)  = &registerAtAnimatorHolder;

Animator::Animator() : lastTime(0),  state(ANIMATOR_FINISHED), onFinish(nullptr) {
	registerAtHolder(this);
}

Animator::~Animator() { }

void Animator::notifyStopped(){
	if (onFinish) {
		onFinish();
	}
}

void Animator::stop(bool runOnFinish) {
	if (!hasFinished()) {
		state = ANIMATOR_STOPPED;
		if(runOnFinish)
			notifyStopped();
	}
}

bool Animator::hasFinished() const {
	return state != ANIMATOR_RUNNING;
}

void Animator::timeShift(timestamp_t offset) {
	lastTime += offset;
}

timestamp_t Animator::getStartingTime() {
	return startingTime;
}

timestamp_t Animator::getLastTime() {
	return lastTime;
}

void Animator::setLastTime(timestamp_t t) {
	lastTime = t;
}

animatorstate_t Animator::getState() {
	return state;
}