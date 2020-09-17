#include "MovingAnimation.h"

offset_t MovingAnimation::getDx(void) const {
	return dx;
}

void MovingAnimation::setDx(offset_t v) {
	dx = v;
}

offset_t MovingAnimation::getDy(void) const {
	return dy;
}

void MovingAnimation::setDy(offset_t v) {
	dy = v;
}

delay_t	MovingAnimation::getDelay(void) const {
	return delay;
}

void MovingAnimation::setDelay(delay_t v) {
	delay = v;
}

bool MovingAnimation::getContinuous(void) const {
	return continuous;
}

void MovingAnimation::setContinuous(bool v) {
	continuous = v;
}

Animation*	MovingAnimation::clone(animid_t newId) const {
	return new MovingAnimation(dx, dy, delay, continuous, newId);
}

MovingAnimation::MovingAnimation(offset_t _dx, offset_t _dy, delay_t _delay, bool c, animid_t _id) :
	dx(_dx), dy(_dy), delay(_delay), continuous(c), Animation(_id) {}