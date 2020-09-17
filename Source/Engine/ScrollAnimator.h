#pragma once
#include "Animator.h"
#include "Sprite.h"
#include "ScrollAnimation.h"
#include "ViewWindow.h"

class ScrollAnimator : public Animator {
private:
	ScrollAnimation		*animation;
	ViewWindow			*view;
	int					scrollIndex;
	TickAnimator		tickAnimator;
	TickAnimation		*tickAnimation;

public:
	ScrollAnimator();
	~ScrollAnimator();

	virtual void progress(timestamp_t currTime) override;
	void start(ViewWindow *v, ScrollAnimation  *a, timestamp_t t);

};