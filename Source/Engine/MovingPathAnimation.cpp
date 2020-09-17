#include "MovingPathAnimation.h"



PathEntry::PathEntry() : dx(0), dy(0), frame(0xff), delay(0){}

PathEntry::PathEntry(offset_t _dx, offset_t _dy, frame_t _frame, delay_t _delay) : dx(_dx), dy(_dy), frame(_frame), delay(_delay){}


PathEntry::PathEntry(const PathEntry &p): dx(p.dx), dy(p.dy), frame(p.frame), delay(p.delay){}

MovingPathAnimation::MovingPathAnimation(const std::vector<PathEntry>& _path, animid_t id, bool c) : path(_path), Animation(id), continuous(c) {}

const std::vector<PathEntry> &MovingPathAnimation::getPath() const {
	return path;
}

void MovingPathAnimation::setPath(const std::vector<PathEntry> &p) {
	path.clear();
	path = p;
}

Animation *MovingPathAnimation::clone(animid_t newId) const {
	return new MovingPathAnimation(path, newId, continuous);
}

bool MovingPathAnimation::getContinuous(void) const {
	return continuous;
}
void MovingPathAnimation::setContinuous(bool v) {
	continuous = v;
}

void MovingPathAnimation::calibratePathEntryFrames(int start, std::vector<int> newFrames) {
	int i = 0;
	for (int j = start; j < path.size(); j++) {
		path[j].frame = newFrames[i];
		i = (i == newFrames.size()-1 ? i : i + 1);
	}
}