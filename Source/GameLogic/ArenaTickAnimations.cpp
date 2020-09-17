#include "ArenaTickAnimations.h"
#include "../Engine/GTypes.h"
#include <iostream>

void courtyardTickFunc(Arena *a) {
	for (int i = 13; i < 30; i++) {
		a->advanceSpriteFrame(i);
	}
}

void throneroomTickFunc(Arena *a, Fighter** f1, Fighter** f2) {
	Dim leftPoint = min(min((*f1)->getSprite()->getX(), (*f2)->getSprite()->getX()), 
		min((*f1)->getSprite()->getX()+ (*f1)->getSprite()->getFrameBox().w, (*f2)->getSprite()->getX() + (*f2)->getSprite()->getFrameBox().w));
	Dim rightPoint = max(max((*f1)->getSprite()->getX(), (*f2)->getSprite()->getX()),
		max((*f1)->getSprite()->getX() + (*f1)->getSprite()->getFrameBox().w, (*f2)->getSprite()->getX() + (*f2)->getSprite()->getFrameBox().w));
	if (leftPoint < 200) a->setSpriteFrame(6, 0);
	else if (leftPoint < 250) a->setSpriteFrame(6, 1);
	else if (leftPoint < 300) a->setSpriteFrame(6, 2);
	else if (leftPoint < 350) a->setSpriteFrame(6, 3);
	else if (leftPoint < 500) a->setSpriteFrame(6, 4);
	else if (rightPoint > 600) a->setSpriteFrame(6, 5);
	else if (rightPoint > 700) a->setSpriteFrame(6, 6);
}