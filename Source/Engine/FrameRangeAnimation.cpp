#include "FrameRangeAnimation.h"

frame_t	FrameRangeAnimation::getStartFrame() const {
	return start;
}

void FrameRangeAnimation::setStartFrame(frame_t f) {
	start = f;
}

frame_t FrameRangeAnimation::getEndFrame() const {
	return end;
}

void FrameRangeAnimation::setEndFrame(frame_t f) {
	end = f;
}

Animation * FrameRangeAnimation::clone(animid_t newId) const {
	return new FrameRangeAnimation(start, end, getDx(), getDy(), getDelay(), getContinuous(), newId);
}

FrameRangeAnimation::FrameRangeAnimation(frame_t s, frame_t e, offset_t dx, offset_t dy, delay_t d, bool c, animid_t id) :
	start(s), end(e), MovingAnimation(dx, dy, d, c, id) {}