#pragma once

#include "MovingAnimation.h"

class FrameRangeAnimation : public MovingAnimation {
private:
	frame_t start, end;
public:
	frame_t		getStartFrame() const;
	void		setStartFrame(frame_t f);
	frame_t		getEndFrame() const;
	void		setEndFrame(frame_t f);
	Animation*	clone(animid_t newId) const;
	FrameRangeAnimation(frame_t s, frame_t e, offset_t dx, offset_t dy, delay_t d, bool c, animid_t id);
};