#include "TickAnimator.h"
#include <iostream>

TickAnimator::TickAnimator() : anim(nullptr), currReps(0) {}

void TickAnimator::start(TickAnimation *a, timestamp_t currTime) {	
	anim = a;
	lastTime = currTime;
	startingTime = currTime;
	state = ANIMATOR_RUNNING;
	currReps = 0;
}

void TickAnimator::progress(timestamp_t currTime) {
	
	while (currTime > lastTime && currTime - lastTime >= anim->getDelay()) {
		anim->executeTickFunc();
		if (anim->getRepetitions() != 0 && currReps == anim->getRepetitions()) {
			state = ANIMATOR_FINISHED;
			notifyStopped();
			break;
		}
		else {
			lastTime += anim->getDelay();
		}		
		currReps++; 
	}
}