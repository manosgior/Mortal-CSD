#pragma once
#include "Animator.h"
#include "Sprite.h"
#include "MovingPathAnimation.h"

class MovingPathAnimator : public Animator {
private:
	Sprite				*sprite;
	MovingPathAnimation *animation;
	int					 pathIndex;

public:
	MovingPathAnimator();
	~MovingPathAnimator();

	virtual void progress(timestamp_t currTime) override;
	void start(Sprite *s, MovingPathAnimation *a, timestamp_t t);
	void setPathIndex(int p);
	int getPathIndex();
	int getMaxIndex();
	MovingPathAnimation* getAnimation();
};