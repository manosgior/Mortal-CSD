#include "AnimationsHolder.h"

std::map<animid_t, MovingAnimation*>		AnimationsHolder::allMovingAnims;
std::map<animid_t, MovingPathAnimation*>	AnimationsHolder::allMovingPathAnims;
std::map<animid_t, FrameRangeAnimation*>	AnimationsHolder::allFrameRangeAnims;
std::map<animid_t, FrameListAnimation*>		AnimationsHolder::allFrameListAnims;
std::map<animid_t, FlashAnimation*>			AnimationsHolder::allFlashAnims;
std::map<animid_t, ScrollAnimation*>		AnimationsHolder::allScrollAnims;


FrameListAnimation* AnimationsHolder::getFrameListAnim(animid_t id) {
	return allFrameListAnims[id];
}

FrameRangeAnimation* AnimationsHolder::getFrameRangeAnim(animid_t id) {
	return allFrameRangeAnims[id];
}

MovingAnimation* AnimationsHolder::getMovingAnim(animid_t id) {
	return allMovingAnims[id];
}

MovingPathAnimation* AnimationsHolder::getMovingPathAnim(animid_t id) {
	return allMovingPathAnims[id];
}

FlashAnimation* AnimationsHolder::getFlashAnim(animid_t id) {
	return allFlashAnims[id];
}

ScrollAnimation* AnimationsHolder::getScrollAnim(animid_t id) {
	return allScrollAnims[id];
}

void AnimationsHolder::addMovingAnimation(MovingAnimation *a) {
	allMovingAnims[a->getID()] = a;
}

void AnimationsHolder::addFrameRangeAnimation(FrameRangeAnimation *a) {
	allFrameRangeAnims[a->getID()] = a;
}

void AnimationsHolder::addFrameListAnimation(FrameListAnimation *a) {
	allFrameListAnims[a->getID()] = a;
}

void AnimationsHolder::addMovingPathAnimation(MovingPathAnimation *a) {
	allMovingPathAnims[a->getID()] = a;
}

void AnimationsHolder::addFlashAnimation(FlashAnimation *a) {
	allFlashAnims[a->getID()] = a;
}

void AnimationsHolder::addScrollAnimation(ScrollAnimation *a) {
	allScrollAnims[a->getID()] = a;
}

void AnimationsHolder::flipAllAndAdd(std::string postfix) {
	std::vector<MovingAnimation *> newMovingAnims;
	for (auto anim : allMovingAnims) {
		auto tmp = (MovingAnimation*)anim.second->clone(anim.first + postfix);
		tmp->setDx(-tmp->getDx());
		newMovingAnims.push_back(tmp);
	}
	for (auto anim : newMovingAnims)
		addMovingAnimation(anim);
	std::vector<FrameRangeAnimation *> newFRAnims;
	for (auto anim : allFrameRangeAnims) {
		auto tmp = (FrameRangeAnimation*)anim.second->clone(anim.first + postfix);
		tmp->setDx(-tmp->getDx());
		newFRAnims.push_back(tmp);
	}
	for (auto anim : newFRAnims)
		addFrameRangeAnimation(anim);
	std::vector<FrameListAnimation *> newFLAnims;
	for (auto anim : allFrameListAnims) {
		auto tmp = (FrameListAnimation*)anim.second->clone(anim.first + postfix);
		tmp->setDx(-tmp->getDx());
		newFLAnims.push_back(tmp);
	}
	for (auto anim : newFLAnims)
		addFrameListAnimation(anim);
	std::vector<MovingPathAnimation *> newMPAnims;
	for (auto anim : allMovingPathAnims) {
		auto tmp = (MovingPathAnimation*)anim.second->clone(anim.first + postfix);
		std::vector<PathEntry> path = anim.second->getPath(), newPath;
		for (auto p : path) {
			p.dx = -p.dx;
			newPath.push_back(p);
		}
		tmp->setPath(newPath);
		newMPAnims.push_back(tmp);
	}
	for (auto anim : newMPAnims)
		addMovingPathAnimation(anim);
}
