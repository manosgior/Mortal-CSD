#include "UI.h"
#include <iostream>

UI::UI(ViewWindow *win, Arena *arena, Fighter *f1, Fighter *f2, timestamp_t *t) : win(win), arena(arena), f1(f1), f2(f2), t(t) {
	bgLayer = new Layer({win->getWidth(), win->getHeight()});
	constructLifebars();
	constructNames();
	constructTimer();
	constructRound();
	digit1Expired = false;
	digit2Expired = false;	
	winnerDisplayed = false;
	winnerBeingDisplayed = false;
	doingFatality = false;
}

void UI::addSpritesToHolder() {
	SpritesHolder h = SpritesHolder::Get();
	for (std::pair<std::string, Sprite*> s : sprites) {
		h.Add(s.second);
	}
}

void UI::removeSpritesFromHolder() {
	SpritesHolder h = SpritesHolder::Get();
	for (std::pair<std::string, Sprite*> s : sprites) {
		h.Remove(s.second);
	}
}

void UI::setFighters(Fighter *f1, Fighter *f2) {
	this->f1 = f1;
	this->f2 = f2;
	for (std::pair<std::string, Sprite*> s : sprites) {
		if (s.first == "subzeroWins" || s.first == "scorpionWins" || s.first == "draw") s.second->setVisibility(false);
	}
}

void UI::constructLifebars() {
	float startX = win->getWidth() * 0.05;
	float startY = win->getHeight() * 0.1;

	SpritesHolder h = SpritesHolder::Get();
	AnimationFilmHolder &filmHolder = AnimationFilmHolder::Get();
	AnimatorHolder animatorHolder;

	Sprite *p1GreenLifebar = new Sprite(startX + 1, startY, 11, bgLayer, filmHolder.GetFilm("greenLifebar"), HFrameAlign::LEFT, VFrameAlign::TOP, "p1GreenLifebar", "full");
	Sprite *p1RedLifebar = new Sprite(startX, startY, 11, bgLayer, filmHolder.GetFilm("redLifebar"), HFrameAlign::LEFT, VFrameAlign::TOP, "p1RedLifebar", "empty");
	Sprite *danger1 = new Sprite(startX, startY + p1GreenLifebar->getFrameBox().h, 11, bgLayer, filmHolder.GetFilm("danger"), HFrameAlign::LEFT, VFrameAlign::TOP, "danger1", "achtung!");
	danger1->setVisibility(false);

	Sprite *p2GreenLifebar = new Sprite(win->getWidth() - startX - p1RedLifebar->getFrameBox().w + 1, startY, 11, bgLayer, filmHolder.GetFilm("greenLifebar"), HFrameAlign::LEFT, VFrameAlign::TOP, "p2GreenLifebar", "full");
	Sprite *p2RedLifebar = new Sprite(win->getWidth() - startX - p1RedLifebar->getFrameBox().w, startY, 11, bgLayer, filmHolder.GetFilm("redLifebar"), HFrameAlign::LEFT, VFrameAlign::TOP, "p2RedLifebar", "empty");
	Sprite *danger2 = new Sprite(win->getWidth() - startX - danger1->getFrameBox().w, startY + p1GreenLifebar->getFrameBox().h, 11, bgLayer, filmHolder.GetFilm("danger"), HFrameAlign::LEFT, VFrameAlign::TOP, "danger2", "achtung!");
	danger2->setVisibility(false);

	sprites["p1Lifebar"] = p1GreenLifebar;
	sprites["p2Lifebar"] = p2GreenLifebar;
	sprites["p1Danger"] = danger1;
	sprites["p2Danger"] = danger2;
	
	h.Add(p1RedLifebar);
	h.Add(p1GreenLifebar);
	h.Add(p2RedLifebar);
	h.Add(p2GreenLifebar);
	h.Add(danger1);
	h.Add(danger2);
	
	TickAnimation *lifebarAnimation1 = new TickAnimation(20, 0, "lifebar1");
	TickAnimation *lifebarAnimation2 = new TickAnimation(20, 0, "lifebar2");

	lifebarAnimation1->setTickFunc<std::function<void(void)>>(std::bind(&leftLifebarTickFunc, f1, p1GreenLifebar, danger1));
	lifebarAnimation2->setTickFunc<std::function<void(void)>>(std::bind(&rightLifebarTickFunc, f2, p2GreenLifebar, danger2));

	TickAnimator  *lifebarAnimator1 = new TickAnimator();
	TickAnimator  *lifebarAnimator2 = new TickAnimator();

	animations["p1Lifebar"].first = lifebarAnimation1;
	animations["p1Lifebar"].second = lifebarAnimator1;
	animations["p2Lifebar"].first = lifebarAnimation2;
	animations["p2Lifebar"].second = lifebarAnimator2;

	lifebarAnimator1->start(lifebarAnimation1, *t);
	lifebarAnimator2->start(lifebarAnimation2, *t);

	animatorHolder.Register(lifebarAnimator1);
	animatorHolder.Register(lifebarAnimator2);

	animatorHolder.markAsRunning(lifebarAnimator1);
	animatorHolder.markAsRunning(lifebarAnimator2);
}

void UI::constructNames() {
	float startX = win->getWidth() * 0.05;
	float startY = win->getHeight() * 0.1;	

	SpritesHolder h = SpritesHolder::Get();
	AnimationFilmHolder &filmHolder = AnimationFilmHolder::Get();
	AnimatorHolder animatorHolder;

	namesToIndex["cage"] = 0;
	namesToIndex["goro"] = 1;
	namesToIndex["cano"] = 2;
	namesToIndex["liukang"] = 3;
	namesToIndex["raiden"] = 4;
	namesToIndex["scorpion"] = 5;
	namesToIndex["shangtsung"] = 6;
	namesToIndex["sonya"] = 7;
	namesToIndex["subzero"] = 8;

	const AnimationFilm *namesFilm = filmHolder.GetFilm("names");
	Sprite *p1 = new Sprite(startX + 10, startY + 2, 11, bgLayer, namesFilm, HFrameAlign::LEFT, VFrameAlign::TOP, "subzeroName", "displaying");
	Sprite *p2 = new Sprite(win->getWidth() - startX - namesFilm->getFrameBox(namesToIndex[f2->getName()]).w - 10, startY + 2, 11, bgLayer, namesFilm, HFrameAlign::LEFT, VFrameAlign::TOP, "scorpionName", "displaying");

	float startWins = win->getWidth() / 2 - filmHolder.GetFilm(f1->getName() + "Wins")->getFrameBox(0).w / 2;
	float startDraw = win->getWidth() / 2 - filmHolder.GetFilm("draw")->getFrameBox(0).w / 2;

	Sprite *p1Wins = new Sprite(startWins, 50, 11, bgLayer, filmHolder.GetFilm(f1->getName() + "Wins"),HFrameAlign::LEFT, VFrameAlign::TOP, "p1Wins", "p1Wins");
	Sprite *p2Wins = new Sprite(startWins, 50, 11, bgLayer, filmHolder.GetFilm(f2->getName() + "Wins"), HFrameAlign::LEFT, VFrameAlign::TOP, "p2Wins", "p2Wins");
	Sprite *subzeroWins = new Sprite(startWins, 50, 11, bgLayer, filmHolder.GetFilm("subzeroWins"), HFrameAlign::LEFT, VFrameAlign::TOP, "p2Wins", "p2Wins");
	Sprite *scorpionWins = new Sprite(startWins, 50, 11, bgLayer, filmHolder.GetFilm("scorpionWins"), HFrameAlign::LEFT, VFrameAlign::TOP, "p2Wins", "p2Wins");
	Sprite *draw   = new Sprite(startDraw, 50, 11, bgLayer, filmHolder.GetFilm("draw"), HFrameAlign::LEFT, VFrameAlign::TOP, "draw", "draw");

	p1Wins->setVisibility(false);
	p2Wins->setVisibility(false);
	draw->setVisibility(false);
	
	sprites["p1Name"] = p1;
	sprites["p2Name"] = p2;

	sprites[f1->getName() + "Wins"] = p1Wins;

	if (f1->getName() == f2->getName()) {
		if (f1->getName() == "scorpion") {
			sprites["subzeroWins"] = subzeroWins;
		}
		else {
			sprites["scorpionWins"] = scorpionWins;
		}
	}
	else {
		sprites[f2->getName()+"Wins"] = p2Wins;
	}
	
	sprites["draw"] = draw;
	
	p1->setFrame(namesToIndex[f1->getName()]);
	p2->setFrame(namesToIndex[f2->getName()]);

	TickAnimation *player1WinsAnimation = new TickAnimation(60, 40, "player1WinsAnimation");
	TickAnimation *player2WinsAnimation = new TickAnimation(60, 40, "player2WinsAnimation");
	TickAnimation *drawAnimation = new TickAnimation(60, 40, "drawAnimation");
	
	player1WinsAnimation->setTickFunc<std::function<void(void)>>(std::bind(&winsTickFunc, p1Wins));
	if (f1->getName() == f2->getName()) {
		if (f1->getName() == "scorpion") {
			player2WinsAnimation->setTickFunc<std::function<void(void)>>(std::bind(&winsTickFunc, subzeroWins));
		}
		else {
			player2WinsAnimation->setTickFunc<std::function<void(void)>>(std::bind(&winsTickFunc, scorpionWins));
		}
	}
	else {
		player2WinsAnimation->setTickFunc<std::function<void(void)>>(std::bind(&winsTickFunc, p2Wins));
	}
	//player2WinsAnimation->setTickFunc<std::function<void(void)>>(std::bind(&winsTickFunc, p2Wins));
	drawAnimation->setTickFunc<std::function<void(void)>>(std::bind(&drawTickFunc, draw));

	TickAnimator  *player1WinsAnimator = new TickAnimator();
	TickAnimator  *player2WinsAnimator = new TickAnimator();	
	TickAnimator  *drawAnimator = new TickAnimator();

	player1WinsAnimator->setOnFinish(std::bind(&UI::setWinnerDisplayed, this, f1));
	player2WinsAnimator->setOnFinish(std::bind(&UI::setWinnerDisplayed, this, f2));
	drawAnimator->setOnFinish(std::bind(&UI::setWinnerDisplayed, this, nullptr));

	animatorHolder.Register(player1WinsAnimator);
	animatorHolder.Register(player2WinsAnimator);
	animatorHolder.Register(drawAnimator);

	animations["subzeroWins"].first = player1WinsAnimation;
	animations["scorpionWins"].first = player2WinsAnimation;
	animations["draw"].first = drawAnimation;

	animations["subzeroWins"].second = player1WinsAnimator;
	animations["scorpionWins"].second = player2WinsAnimator;
	animations["draw"].second = drawAnimator;
	
	h.Add(p1);
	h.Add(p2);
	h.Add(p1Wins);
	h.Add(p2Wins);
	h.Add(draw);
	
}

void UI::constructTimer() {
	SpritesHolder h = SpritesHolder::Get();
	AnimationFilmHolder &filmHolder = AnimationFilmHolder::Get();
	AnimatorHolder animatorHolder;
	
	float startX = win->getWidth() / 2 - filmHolder.GetFilm("timer")->getFrameBox(0).w;

	Sprite *d1 = new Sprite(startX, 5, 11, bgLayer, filmHolder.GetFilm("timer"), HFrameAlign::LEFT, VFrameAlign::TOP, "timer", "ticking");
	Sprite *d2 = new Sprite(startX + d1->getFrameBox().w, 5, 11, bgLayer, filmHolder.GetFilm("timer"), HFrameAlign::LEFT, VFrameAlign::TOP, "timer", "ticking");
	d1->setFrame(9);
	d2->setFrame(9);

	sprites["firstDecimal"] = d1;
	sprites["secondDecimal"] = d2;
	
	h.Add(d1);
	h.Add(d2);
	
	TickAnimation *timerAnimation1 = new TickAnimation(10000, 8, "timerAnimation1");
	TickAnimation *timerAnimation2 = new TickAnimation(1000, 98, "timerAnimation2");

	timerAnimation1->setTickFunc<std::function<void(void)>>(std::bind(&timerTickFunc, d1));
	timerAnimation2->setTickFunc<std::function<void(void)>>(std::bind(&timerTickFunc, d2));

	TickAnimator  *timerAnimator1 = new TickAnimator();
	TickAnimator  *timerAnimator2 = new TickAnimator();
	
	timerAnimator1->setOnFinish(std::bind(&UI::digit1HasExpired, this));
	timerAnimator2->setOnFinish(std::bind(&UI::digit2HasExpired, this));

	animations["firstDecimal"].first = timerAnimation1;
	animations["firstDecimal"].second = timerAnimator1;
	animations["secondDecimal"].first = timerAnimation2;
	animations["secondDecimal"].second = timerAnimator2;	
	//timerAnimator2->setOnFinish();

	animatorHolder.Register(timerAnimator1);
	animatorHolder.Register(timerAnimator2);	
}

void UI::constructRound() {
	SpritesHolder h = SpritesHolder::Get();
	AnimationFilmHolder &filmHolder = AnimationFilmHolder::Get();
	AnimatorHolder animatorHolder;

	float startRound = win->getWidth() / 2 - filmHolder.GetFilm("round")->getFrameBox(0).w / 2;
	float startFight = win->getWidth() / 2 - filmHolder.GetFilm("fight")->getFrameBox(0).w / 2;
	float startFinishHim = win->getWidth() / 2 - filmHolder.GetFilm("finishHim")->getFrameBox(0).w / 2;
	Sprite *round = new Sprite(startRound, 50, 11, bgLayer, filmHolder.GetFilm("round"), HFrameAlign::LEFT, VFrameAlign::TOP, "round", "round");
	Sprite *fight = new Sprite(startFight, 50, 11, bgLayer, filmHolder.GetFilm("fight"), HFrameAlign::LEFT, VFrameAlign::TOP, "fight", "fight");
	Sprite *finishHim = new Sprite(startFinishHim, 50, 11, bgLayer, filmHolder.GetFilm("finishHim"), HFrameAlign::LEFT, VFrameAlign::TOP, "finishHim", "finishHim");
		
	round->setVisibility(false);
	fight->setVisibility(false);
	finishHim->setVisibility(false);
	
	h.Add(round);
	h.Add(fight);
	h.Add(finishHim);
	
	TickAnimation *roundAnimation = new TickAnimation(1500, 1, "roundAnimation");
	TickAnimation *fightAnimation = new TickAnimation(50, 20, "fightAnimation");
	TickAnimation *finishHimAnimation = new TickAnimation(50, 20, "finishHimAnimation");

	roundAnimation->setTickFunc<std::function<void(void)>>(std::bind(&roundTickFunc, round));
	fightAnimation->setTickFunc<std::function<void(void)>>(std::bind(&fightTickFunc, fight));
	finishHimAnimation->setTickFunc<std::function<void(void)>>(std::bind(&finishHimTickFunc, finishHim));

	TickAnimator  *roundAnimator = new TickAnimator();
	TickAnimator  *fightAnimator = new TickAnimator();
	TickAnimator  *finishHimAnimator = new TickAnimator();
	
	roundAnimator->setOnFinish(std::bind(&UI::displayFight, this, t));
	fightAnimator->setOnFinish(std::bind(&UI::displayStartRound, this, t));
	finishHimAnimator->setOnFinish(std::bind(&Sprite::setVisibility, finishHim, false));

	animatorHolder.Register(roundAnimator);
	animatorHolder.Register(fightAnimator);
	animatorHolder.Register(finishHimAnimator);

	sprites["round"] = round;
	animations["round"].first = roundAnimation;
	animations["round"].second = roundAnimator;

	sprites["fight"] = fight;
	animations["fight"].first = fightAnimation;
	animations["fight"].second = fightAnimator;

	sprites["finishHim"] = finishHim;
	animations["finishHim"].first = finishHimAnimation;
	animations["finishHim"].second = finishHimAnimator;
}

void UI::resetUI() {
	float startX = win->getWidth() * 0.05;
	win->resetInitialCoordinates();

	sprites["p1Name"]->setFrame(namesToIndex[f1->getName()]);
	sprites["p2Name"]->setFrame(namesToIndex[f2->getName()]);
	sprites["p1Lifebar"]->setFrame(0);
	sprites["p2Lifebar"]->setFrame(0);
	sprites["p2Lifebar"]->setX(win->getWidth() - startX - sprites["p1Lifebar"]->getFrameBox().w + 1);
	sprites["firstDecimal"]->setFrame(9);
	sprites["secondDecimal"]->setFrame(9);
	sprites["p1Danger"]->setVisibility(false);
	sprites["p2Danger"]->setVisibility(false);
	digit1Expired = digit2Expired = false;
}

void UI::displayRound(timestamp_t *t, int round) {	
	winnerDisplayed = false;
	winnerBeingDisplayed = false;
	doingFatality = false;
	sprites["round"]->setFrame(round);
	sprites["round"]->setVisibility(true);
	this->t = t;
	animations["round"].second->start(animations["round"].first, *t);

	AnimatorHolder animatorHolder;
	animatorHolder.markAsRunning(animations["round"].second);

	resetUI();
	f1->setHP(150);
	f2->setHP(150);
	f1->setLastHP(150);
	f2->setLastHP(150);	
	f1->setState("idle");
	f2->setState("idle");
	f1->setNextAction("idle");
	f2->setNextAction("idle");
	f1->startPlayingAnimation("idle", *t);
	f2->startPlayingAnimation("idle", *t);	
	f1->getSprite()->setX(arena->getFighterPos(0));
	f2->getSprite()->setX(arena->getFighterPos(2));
	f1->getSprite()->setY(arena->getHeight() - arena->getFighterPos(1) - f1->getSprite()->getFrameBox().h);
	f2->getSprite()->setY(arena->getHeight() - arena->getFighterPos(1) - f2->getSprite()->getFrameBox().h);
}

void UI::displayFight(timestamp_t *t) {
	sprites["round"]->setVisibility(false);
	sprites["fight"]->setVisibility(true);
	this->t = t;	

	animations["fight"].second->start(animations["fight"].first, *t);
	AudioHolder::playRepeatedSound(arenaName);

	AudioHolder::playSound("fight");
	AnimatorHolder animatorHolder;	
	animatorHolder.markAsRunning(animations["fight"].second);
}

void UI::displayStartRound(timestamp_t *t) {
	sprites["fight"]->setVisibility(false);

	animations["firstDecimal"].second->start(animations["firstDecimal"].first, *t);
	animations["secondDecimal"].second->start(animations["secondDecimal"].first, *t);

	f1->getSprite()->setY(arena->getHeight() - arena->getFighterPos(1) - f1->getSprite()->getFrameBox().h);
	f2->getSprite()->setY(arena->getHeight() - arena->getFighterPos(3) - f2->getSprite()->getFrameBox().h);
	
	AnimatorHolder animatorHolder;
	animatorHolder.markAsRunning(animations["firstDecimal"].second);
	animatorHolder.markAsRunning(animations["secondDecimal"].second);	
	animatorHolder.markAsRunning(animations["p1Lifebar"].second);
	animatorHolder.markAsRunning(animations["p2Lifebar"].second);
	f1->setIgnoreInput(false);
	f2->setIgnoreInput(false);
}

bool UI::timerHasExpired() {	
	return digit1Expired && digit2Expired;
}

void UI::digit1HasExpired() {
	digit1Expired = true;
}

void UI::digit2HasExpired() {
	digit2Expired = true;
}

void UI::displayFinishHim(Fighter *f) {
	//f->setHP(1);
	//f->setLastHP(1);
	//winnerBeingDisplayed = true;	
	
	doingFatality = true;
	sprites["finishHim"]->setVisibility(true);
	animations["finishHim"].second->start(animations["finishHim"].first, *t);
	AudioHolder::playSound("finishHim");
	AnimatorHolder animatorHolder;
	animatorHolder.markAsRunning(animations["finishHim"].second);
}

void UI::displayWinner(Fighter *f) {
	animations["firstDecimal"].second->stop(false);
	animations["secondDecimal"].second->stop(false);
	winnerBeingDisplayed = true;
	sprites[f->getName() + "Wins"]->setVisibility(true);	
	AudioHolder::playSound(f->getName()+"Wins");
	AudioHolder::stopPlayingSound(arenaName);

	animations[f->getName() + "Wins"].second->start(animations[f->getName() + "Wins"].first, *t);

	AnimatorHolder animatorHolder;
	animatorHolder.markAsRunning(animations[f->getName() + "Wins"].second);	
	f == f1 ? f2->incLosses() : f1->incLosses();
}

void UI::stopWins() {
	sprites["scorpionWins"]->setVisibility(false);
	sprites["subzeroWins"]->setVisibility(false);
}

void UI::displayDraw() {
	animations["firstDecimal"].second->stop(false);
	animations["secondDecimal"].second->stop(false);
	winnerBeingDisplayed = true;

	sprites["draw"]->setVisibility(true);
	animations["draw"].second->start(animations["draw"].first, *t);

	AnimatorHolder animatorHolder;
	animatorHolder.markAsRunning(animations["draw"].second);	
}

void UI::setWinnerDisplayed(Fighter *f) {
	if (f != nullptr) sprites[f->getName() + "Wins"]->setVisibility(false);
	else sprites["draw"]->setVisibility(false);

	winnerDisplayed = true;	
	
}

bool UI::winnerIsDisplayed() {
	return winnerDisplayed;
}

bool UI::winnerIsBeingDisplayed() {
	return winnerBeingDisplayed;
}

bool UI::fatality() {
	return doingFatality;
}

void UI::setArenaName(std::string s) {
	arenaName = s;
}

std::string UI::getArenaname() {
	return arenaName;
}