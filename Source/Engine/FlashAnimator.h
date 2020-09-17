#pragma once

#include "Animator.h"
#include "Sprite.h"
#include "Animation.h"
#include "FlashAnimation.h"

class FlashAnimator : public Animator {
	Sprite *		sprite;
	FlashAnimation* anim;
	int				currReps;
	bool			currentlyVisible;

public:
	void			progress(timestamp_t currTime);
	void			start(Sprite* s, FlashAnimation* a, timestamp_t t);
	Sprite*			getSprite();
	void			setSprite(Sprite* s);
	FlashAnimation* getAnim();
	void			setAnim(FlashAnimation* a);
	FlashAnimator(void);
	~FlashAnimator();
};