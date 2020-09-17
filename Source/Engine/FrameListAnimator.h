#pragma once

#include "Animator.h"
#include "Sprite.h"
#include "FrameListAnimation.h"

class FrameListAnimator : public Animator {
private:
	Sprite*				sprite;
	FrameListAnimation*		anim;
	frame_t					currFrame;
public:
	void progress(timestamp_t currTime) override;
	void start(Sprite* s, FrameListAnimation* a, timestamp_t t);
	FrameListAnimator();
	~FrameListAnimator();
};
