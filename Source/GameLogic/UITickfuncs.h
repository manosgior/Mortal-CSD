#pragma once

#include "Fighter.h"

void leftLifebarTickFunc(Fighter *fighter, Sprite *s, Sprite *danger);

void rightLifebarTickFunc(Fighter *fighter, Sprite *s, Sprite *danger);

void timerTickFunc(Sprite *s);

void roundTickFunc(Sprite *s);

void fightTickFunc(Sprite *s);

void winsTickFunc(Sprite *s);

void drawTickFunc(Sprite *s);

void finishHimTickFunc(Sprite *s);