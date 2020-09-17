#pragma once

#include "MovingAnimation.h"
#include <vector>

class FrameListAnimation : public MovingAnimation {
private:
	std::vector<frame_t> frames;
public:
	const std::vector<frame_t>&	getFrames() const;
	void						setFrames(const std::vector<frame_t>& f);
	Animation*					clone(animid_t newId) const;
	FrameListAnimation(const std::vector<frame_t>& frames, offset_t dx, offset_t dy, delay_t d, bool c, animid_t id);
};
