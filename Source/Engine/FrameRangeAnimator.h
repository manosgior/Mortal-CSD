#pragma once

#include "Animator.h"
#include "Sprite.h"
#include "FrameRangeAnimation.h"

class FrameRangeAnimator : public Animator {
private:
	Sprite*					sprite;
	FrameRangeAnimation*	anim;
	frame_t					currFrame;
public:
	void progress(timestamp_t currTime);
	void start(Sprite* s, FrameRangeAnimation* a, timestamp_t t);
	FrameRangeAnimator();
	~FrameRangeAnimator();
};
