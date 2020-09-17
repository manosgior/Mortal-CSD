#include "MovingAnimator.h"

void MovingAnimator::progress(timestamp_t currTime) {
	while (currTime > lastTime && currTime - lastTime >= anim->getDelay()) {
		sprite->move(anim->getDx(), anim->getDy());
		if (!anim->getContinuous()) {
			state = ANIMATOR_FINISHED;
			notifyStopped();
			break;
		}
		else {
			lastTime += anim->getDelay();
		}
	}
}

void MovingAnimator::start(Sprite* s, MovingAnimation* a, timestamp_t t) {
	sprite = s;
	anim = a;
	lastTime = t;
	startingTime = t;
	state = ANIMATOR_RUNNING;
}

Sprite*	MovingAnimator::getSprite() {
	return sprite;
}

void MovingAnimator::setSprite(Sprite* s) {
	sprite = s;
}

MovingAnimation* MovingAnimator::getAnim() {
	return anim;
}

void MovingAnimator::setAnim(MovingAnimation* a) {
	anim = a;
}

MovingAnimator::MovingAnimator(void) :
	sprite((Sprite*)0), anim((MovingAnimation *)0) {}

MovingAnimator::~MovingAnimator() {}