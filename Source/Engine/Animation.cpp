#include "Animation.h"

Animation::Animation(animid_t _id) : id(_id){}

animid_t Animation::getID(void) {
	return id;
}

Animation::~Animation() {}