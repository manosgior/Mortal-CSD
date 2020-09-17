#include "UITickfuncs.h"
#include <iostream>

void leftLifebarTickFunc(Fighter *fighter, Sprite *lifebar, Sprite *danger) {	
	int hp = fighter->getHP();
	int lastHp = fighter->getLastHP();
	
	if (lastHp >=0 && lastHp != hp) {		
		lifebar->setFrame(lifebar->getFrame() + 1);		
		fighter->setLastHP(lastHp - 1);		
	}
	if (lastHp < 25) {
		if (!danger->isVisible()) {
			AudioHolder::playSound("kingLaugh");
		}
		danger->setVisibility(true);
	}
	else
		danger->setVisibility(false);
}

void rightLifebarTickFunc(Fighter *fighter, Sprite *lifebar, Sprite *danger) {
	int hp = fighter->getHP();
	int lastHp = fighter->getLastHP();
	
	if (lastHp >= 0 && lastHp != hp) {
		lifebar->setFrame(lifebar->getFrame() + 1);
		lifebar->move(1, 0);
		fighter->setLastHP(lastHp - 1);
	}
	
	if (lastHp < 25) {
		if (!danger->isVisible()) {
			AudioHolder::playSound("kingLaugh");
		}
		danger->setVisibility(true);
	}
}
void timerTickFunc(Sprite *s) {
	int index = s->getFrame();

	if (index == 0) s->setFrame(9);
	else s->setFrame(index - 1);
}

void roundTickFunc(Sprite *s) {}

void fightTickFunc(Sprite *s) {
	if (s->getFrame() == s->getFilm()->getTotalFrames() - 1) {
		s->setFrame(0);
	}
	else {
		s->setFrame(s->getFrame() + 1);
	}	
}

void winsTickFunc(Sprite *s) {
	s->getFrame() == 0 ? s->setFrame(1) : s->setFrame(0);	
}

void drawTickFunc(Sprite *s) {
	s->getFrame() == 0 ? s->setFrame(1) : s->setFrame(0);
}

void finishHimTickFunc(Sprite *s) {
	if (s->getFrame() == s->getFilm()->getTotalFrames() - 1) {
		s->setFrame(0);
	}
	else {
		s->setFrame(s->getFrame() + 1);
	}
}