#include "FlashAnimation.h"

void FlashAnimation::setRepetitions(frame_t n) {
	repetitions = n;
}

frame_t FlashAnimation::getRepetitions(void) const {
	return repetitions;
}

void	FlashAnimation::setHideDelay(delay_t d) {
	hideDelay = d;
}

delay_t	FlashAnimation::getHideDelay(void) {
	return hideDelay;
}

void	FlashAnimation::setShowDelay(delay_t d) {
	showDelay = d;
}

delay_t FlashAnimation::getShowDelay(void) const {
	return showDelay;
}

Animation* FlashAnimation::clone(animid_t newId) const {
	return new FlashAnimation(repetitions, hideDelay, showDelay, newId);
}

FlashAnimation::FlashAnimation(frame_t n, delay_t show, delay_t hide, animid_t id) :
	repetitions(n), hideDelay(hide), showDelay(show), Animation(id) {}