#pragma once

#include "Animator.h"
#include "TickAnimation.h"


class TickAnimator : public Animator {
private:
	TickAnimation *anim;
	int currReps;
public:
	TickAnimator();
	void start(TickAnimation *a, timestamp_t currTime);
	virtual void progress(timestamp_t currTime) override;

};