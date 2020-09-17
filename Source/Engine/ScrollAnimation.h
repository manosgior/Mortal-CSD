#pragma once
#include "Animation.h"
#include <vector>

struct ScrollEntry {
	offset_t	horOffset;
	offset_t	verOffset;
	delay_t		delay;
};

class ScrollAnimation : public Animation {
	std::vector<ScrollEntry>	scroll;
	bool continuous;

public:
	ScrollAnimation(const std::vector<ScrollEntry>& _scroll, animid_t id, bool c);

	const std::vector<ScrollEntry> &getScroll() const;
	void setScroll(const std::vector<ScrollEntry>& p);
	Animation* clone(animid_t newId) const;

	void setContinuous(bool c);
	bool getContinuous();
};