#include "FrameRangeAnimator.h"

void FrameRangeAnimator::progress(timestamp_t currTime) {
	while (currTime > lastTime && currTime - lastTime >= anim->getDelay()) {
		if (currFrame == anim->getEndFrame()) currFrame = anim->getStartFrame();
		else currFrame++;

		sprite->move(anim->getDx(), anim->getDy());
		sprite->setFrame(currFrame);
		lastTime += anim->getDelay();

		if (currFrame == anim->getEndFrame() && !anim->getContinuous()) {
			state = ANIMATOR_FINISHED;
			notifyStopped();
			return;
		}
	}
}

void FrameRangeAnimator::start(Sprite* s, FrameRangeAnimation* a, timestamp_t t) {
	sprite = s;
	anim = a;
	lastTime = t;
	startingTime = t;
	state = ANIMATOR_RUNNING;
	sprite->setFrame(currFrame = anim->getStartFrame());
}

FrameRangeAnimator::FrameRangeAnimator() : sprite(nullptr), anim(nullptr), currFrame(0xFF) {}

FrameRangeAnimator::~FrameRangeAnimator() {}