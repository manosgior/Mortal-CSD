#include "ScrollAnimation.h"


ScrollAnimation::ScrollAnimation(const std::vector<ScrollEntry>& _scroll, animid_t id, bool c) : scroll(_scroll), Animation(id), continuous(c) { }

const std::vector<ScrollEntry> &ScrollAnimation::getScroll() const {
	return scroll;
}

void ScrollAnimation::setScroll(const std::vector<ScrollEntry>& p){
	scroll.clear(); 
	scroll = p;
}

Animation* ScrollAnimation::clone(animid_t newId) const{
	return new ScrollAnimation(scroll, newId, continuous);
}

void ScrollAnimation::setContinuous(bool c) {
	continuous = c;
}

bool ScrollAnimation::getContinuous() {
	return continuous;
}