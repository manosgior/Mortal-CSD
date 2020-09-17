#pragma once
#include "FrameListAnimation.h"
#include "FrameRangeAnimation.h"
#include "MovingAnimation.h"
#include "MovingPathAnimation.h"
#include "FlashAnimation.h"
#include "ScrollAnimation.h"
#include <string>
#include <map>

class AnimationsHolder {
private:
	AnimationsHolder();
	~AnimationsHolder() {} ;
	static std::map<animid_t, MovingAnimation*>		allMovingAnims;
	static std::map<animid_t, MovingPathAnimation*>	allMovingPathAnims;
	static std::map<animid_t, FrameRangeAnimation*>	allFrameRangeAnims;
	static std::map<animid_t, FrameListAnimation*>	allFrameListAnims;
	static std::map<animid_t, FlashAnimation*>		allFlashAnims;
	static std::map<animid_t, ScrollAnimation*>		allScrollAnims;
public:
	static FrameListAnimation* getFrameListAnim(animid_t id);
	static FrameRangeAnimation* getFrameRangeAnim(animid_t id);
	static MovingAnimation* getMovingAnim(animid_t id);
	static MovingPathAnimation* getMovingPathAnim(animid_t id);
	static FlashAnimation* getFlashAnim(animid_t id);
	static ScrollAnimation* getScrollAnim(animid_t id);

	static void addMovingAnimation(MovingAnimation*);
	static void addFrameRangeAnimation(FrameRangeAnimation*);
	static void addFrameListAnimation(FrameListAnimation*);
	static void addMovingPathAnimation(MovingPathAnimation*);
	static void addFlashAnimation(FlashAnimation*);
	static void addScrollAnimation(ScrollAnimation*);

	static void flipAllAndAdd(std::string postfix);
};