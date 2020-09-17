#include "FrameListAnimation.h"

const std::vector<frame_t>& FrameListAnimation::getFrames() const {
	return frames;
}

void FrameListAnimation::setFrames(const std::vector<frame_t>& f) {
	frames.clear();
	frames = f;
}

Animation* FrameListAnimation::clone(animid_t newId) const {
	return new FrameListAnimation(frames, getDx(), getDy(), getDelay(), getContinuous(), newId);
}

FrameListAnimation::FrameListAnimation(const std::vector<frame_t>& frames, offset_t dx, offset_t dy, delay_t d, bool c, animid_t id) : frames(frames), MovingAnimation(dx, dy, d, c, id) {}