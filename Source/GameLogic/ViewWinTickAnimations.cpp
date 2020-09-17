#include "ViewWinTickAnimations.h"
#include <iostream>


void scrollTickFunc(Fighter **f1, Fighter **f2, ViewWindow *v, Arena *a) {
	int scram = 5;
	int sx1 = (*f1)->getSprite()->getX();
	int sx2 = (*f2)->getSprite()->getX();
	int sy1 = (*f1)->getSprite()->getY();
	int sy2 = (*f2)->getSprite()->getY();
	int sw1 = (*f1)->getSprite()->getFrameBox().w;
	int sh1 = (*f1)->getSprite()->getFrameBox().h;
	int sw2 = (*f2)->getSprite()->getFrameBox().w;
	int sh2 = (*f2)->getSprite()->getFrameBox().h;
	int westPoint = min(sx1, sx2);
	int northPoint = min(sy1, sy2);
	int eastPoint = max(sx1 + sw1, sx2 + sw2);
	int southPoint = max(sy1 + sh1, sy2 + sh2);
	int vx = v->getX();
	int vy = v->getY();
	int vw = v->getWidth();
	int vh = v->getHeight();
	if (eastPoint > vx + (0.9*vw) && westPoint > vx + scram && vx + vw < a->getWidth() - a->getViewBorder(2)) {
		(*f1)->getSprite()->setBorder(2, min(a->getWidth() - a->getBorder(2), vx + vw + scram));
		(*f2)->getSprite()->setBorder(2, min(a->getWidth() - a->getBorder(2), vx + vw + scram));
		v->scroll(scram, 0);	
	}
	if ((northPoint < (vy + 0.2*vh)) && vy > a->getViewBorder(1)) {
		v->scroll(0, -scram);
	} 
	if (westPoint < vx + (0.1*vw) && eastPoint < vx + vw - scram && vx > a->getViewBorder(0)) {
		(*f1)->getSprite()->setBorder(0, max(a->getBorder(0), vx - scram));
		(*f2)->getSprite()->setBorder(0, max(a->getBorder(0), vx - scram));
		v->scroll(-scram, 0);
	
	}
	if ((northPoint > (vy + 0.3*vh)) &&  vy+vh < a->getHeight() - a->getViewBorder(3)) {
		v->scroll(0, scram);
	}
}