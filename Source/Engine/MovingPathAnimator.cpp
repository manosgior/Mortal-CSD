#include "MovingPathAnimator.h"



MovingPathAnimator::MovingPathAnimator() : sprite(nullptr), animation(nullptr), pathIndex(0){ }


MovingPathAnimator::~MovingPathAnimator(){ }
#include <iostream>
void MovingPathAnimator::progress(timestamp_t currTime) {
	std::vector<PathEntry> path = animation->getPath();
	while (currTime > lastTime && currTime - lastTime >= path[pathIndex].delay) {
		sprite->move(path[pathIndex].dx, path[pathIndex].dy);
		sprite->setFrame(path[pathIndex].frame);
		lastTime += path[pathIndex].delay;
		pathIndex++;
		
		if(pathIndex == path.size() && animation->getContinuous())
			pathIndex = 0;
		else if(pathIndex == path.size()){			
			state = ANIMATOR_FINISHED;
			notifyStopped();
			return;
		}
	}
}

void MovingPathAnimator::start(Sprite *s, MovingPathAnimation *a, timestamp_t t) {
	sprite = s;
	animation = a;
	lastTime = t;
	startingTime = t;
	pathIndex = 0;
	state = ANIMATOR_RUNNING;
}

void MovingPathAnimator::setPathIndex(int p) {
	pathIndex = p;
}

int MovingPathAnimator::getPathIndex() {
	return pathIndex;
}

int MovingPathAnimator::getMaxIndex() {
	return animation->getPath().size();
}

MovingPathAnimation* MovingPathAnimator::getAnimation() {
	return animation;
}