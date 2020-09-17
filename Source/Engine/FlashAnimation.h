#pragma once
#include "Animation.h"

class FlashAnimation : public Animation {
	frame_t repetitions;
	delay_t hideDelay;
	delay_t showDelay;
public:
	void setRepetitions(frame_t n);
	frame_t getRepetitions(void) const;
	void	setHideDelay(delay_t d);
	delay_t	getHideDelay(void);
	void	setShowDelay(delay_t d);
	delay_t getShowDelay(void) const;

	Animation* clone(animid_t newId) const;

	FlashAnimation(frame_t n, delay_t show, delay_t hide, animid_t id);
};