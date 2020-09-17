#include "FrameListAnimator.h"

void FrameListAnimator::progress(timestamp_t currTime) {
	std::vector<frame_t> frames = anim->getFrames();

	while (currTime > lastTime && currTime - lastTime >= anim->getDelay()) {
		if (currFrame == frames.size() - 1) 
			currFrame = 0;
		else 
			currFrame++;

		sprite->move(anim->getDx(), anim->getDy());
		sprite->setFrame(frames[currFrame]);
		lastTime += anim->getDelay();

		if (currFrame == frames.size() - 1 && !anim->getContinuous()) {
			state = ANIMATOR_FINISHED;
			notifyStopped();
			return;
		}
	}
}

void FrameListAnimator::start(Sprite* s, FrameListAnimation* a, timestamp_t t) {
	sprite = s;
	anim = a;
	lastTime = t;
	startingTime = t;
	state = ANIMATOR_RUNNING;
	sprite->setFrame(currFrame = *(anim->getFrames().begin()));
}

FrameListAnimator::FrameListAnimator() : sprite(nullptr), anim(nullptr), currFrame(0xFF) {}

FrameListAnimator::~FrameListAnimator() {}