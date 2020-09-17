#include "ScrollAnimator.h"
#include <iostream>


ScrollAnimator::ScrollAnimator() : scrollIndex(0) {}

ScrollAnimator::~ScrollAnimator() {}

void ScrollAnimator::progress(timestamp_t currTime) {
	std::vector<ScrollEntry> scrolls = animation->getScroll();
	while (currTime > lastTime && currTime - lastTime >= scrolls[scrollIndex].delay) {
		view->scroll(scrolls[scrollIndex].horOffset, scrolls[scrollIndex].verOffset);
		lastTime += scrolls[scrollIndex].delay;
		if (scrollIndex < scrolls.size() - 1)
			scrollIndex++;
		else if (animation->getContinuous())
			scrollIndex = 0;
		else {
			state = ANIMATOR_FINISHED;
			notifyStopped();
			return;
		}
	}
}

void ScrollAnimator::start(ViewWindow *v, ScrollAnimation  *a, timestamp_t t) {
	animation = a;
	view = v;
	lastTime = t;
	startingTime = t;
	state = ANIMATOR_RUNNING;
}
