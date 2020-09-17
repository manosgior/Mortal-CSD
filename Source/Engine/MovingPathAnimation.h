#pragma once
#include "Animation.h"
#include <vector>

struct PathEntry {
	offset_t	dx, dy;
	frame_t		frame;
	delay_t		delay;
	PathEntry();
	PathEntry(offset_t _dx, offset_t _dy, frame_t _frame, delay_t _delay);
	PathEntry(const PathEntry& p);
};

class MovingPathAnimation : public Animation{
	std::vector<PathEntry>	path;
	bool					continuous;

public:
	MovingPathAnimation(const std::vector<PathEntry>& _path, animid_t id, bool c);
	bool		getContinuous(void) const;
	void		setContinuous(bool v);
	const		std::vector<PathEntry> &getPath() const;
	void		setPath(const std::vector<PathEntry>& p);
	void		calibratePathEntryFrames(int, std::vector<int>);
	Animation	*clone(animid_t newId) const override;
};