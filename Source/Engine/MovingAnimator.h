#pragma once
#include "Animator.h"
#include "MovingAnimation.h"
#include "Sprite.h"

class MovingAnimator : public Animator {
	Sprite*				sprite;
	MovingAnimation*	anim;

public:
	void			progress(timestamp_t currTime) override;
	void			start(Sprite* s, MovingAnimation* a, timestamp_t t);
	Sprite*			getSprite();
	void			setSprite(Sprite* s);
	MovingAnimation* getAnim();
	void			setAnim(MovingAnimation* a);
	MovingAnimator(void);
	~MovingAnimator();
};