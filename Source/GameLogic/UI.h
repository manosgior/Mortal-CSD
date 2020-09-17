#pragma once

#include "../Engine/TickAnimation.h"
#include "../Engine/TickAnimator.h"
#include "Fighter.h"
#include "../Engine/SpritesHolder.h"
#include "../Engine/AnimationFilmHolder.h"
#include "UITickfuncs.h"
#include "Arena.h"
#include <functional>


class UI {
private:
	ViewWindow						*win;
	Layer							*bgLayer;
	Fighter							*f1;
	Fighter							*f2;
	Arena							*arena;
	bool							digit1Expired;	
	bool							digit2Expired;	
	bool							winnerDisplayed;
	bool							winnerBeingDisplayed;
	bool							doingFatality;
	std::map<std::string, Sprite *> sprites;
	std::map<std::string, std::pair<TickAnimation *, TickAnimator *>> animations;
	std::map<std::string, int>		namesToIndex;
	timestamp_t						*t;

	std::string						arenaName;

	void digit1HasExpired();
	void digit2HasExpired();
	void setWinnerDisplayed(Fighter *f);
public:
	void constructLifebars();
	void constructNames();
	void constructTimer();
	void constructRound();
	void setFighters(Fighter*, Fighter*);
	void resetUI();
	void addSpritesToHolder();
	void removeSpritesFromHolder();
	void stopWins();
	bool timerHasExpired();	
	void displayWinner(Fighter *f);
	void displayRound(timestamp_t *t, int round);
	void displayFight(timestamp_t *t);
	void displayDraw();
	void displayFinishHim(Fighter *f);
	void displayStartRound(timestamp_t *t);
	bool winnerIsBeingDisplayed();
	bool winnerIsDisplayed();	
	bool fatality();
	void setArenaName(std::string);
	std::string getArenaname();
	UI(ViewWindow *win, Arena *arena, Fighter *f1, Fighter *f2, timestamp_t *t);
};

