#include "FlashAnimator.h"
#include <iostream>

void FlashAnimator::progress(timestamp_t currTime) {
	while (currTime > lastTime && (!currentlyVisible ? (currTime - lastTime >= anim->getHideDelay()) : (currTime - lastTime >= anim->getShowDelay()) ) ) {
		sprite->setVisibility(currentlyVisible);
		lastTime += currentlyVisible ? anim->getShowDelay() : anim->getHideDelay();
		currentlyVisible = !currentlyVisible;
		if(!currentlyVisible)
			currReps++;
		if (anim->getRepetitions() != 0 && currReps == anim->getRepetitions()) {
			state = ANIMATOR_FINISHED;
			notifyStopped();
			return;
		}
	}
}

void FlashAnimator::start(Sprite* s, FlashAnimation* a, timestamp_t t) {
	sprite = s;
	anim = a;
	lastTime = t;
	startingTime = t;
	state = ANIMATOR_RUNNING;
}

Sprite*	FlashAnimator::getSprite() {
	return sprite;
}

void FlashAnimator::setSprite(Sprite* s) {
	sprite = s;
}

FlashAnimation* FlashAnimator::getAnim() {
	return anim;
}

void FlashAnimator::setAnim(FlashAnimation* a) {
	anim = a;
}

FlashAnimator::FlashAnimator(void) :
	sprite((Sprite *) 0), anim((FlashAnimation*) 0), currReps(0), currentlyVisible(false) {}
FlashAnimator::~FlashAnimator() {}