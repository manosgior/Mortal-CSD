#include "Fighter.h"
#include "../Engine/SpritesHolder.h"
#include <iostream>
#include <fstream>

FighterStateType stringToStateTypeEnum(std::string s) {
	std::map<std::string, FighterStateType> m = { { "HI_DEF", FighterStateType::HI_DEF },
	{ "LO_DEF", FighterStateType::LO_DEF },{ "HI_NEU", FighterStateType::HI_NEU },
	{ "LO_NEU", FighterStateType::LO_NEU },{ "HI_ATK", FighterStateType::HI_ATK },
	{ "LO_ATK", FighterStateType::LO_ATK },{ "JMP", FighterStateType::JMP },
	{ "HI_HIT", FighterStateType::HI_HIT },{ "LO_HIT",  FighterStateType::LO_HIT} };
	return m[s];
}

bool stateIsAtk(FighterStateType st) {
	return st == FighterStateType::LO_ATK || st == FighterStateType::HI_ATK;
}

bool stateIsNeu(FighterStateType st) {
	return st == FighterStateType::HI_NEU || st == FighterStateType::LO_NEU;
}

bool stateIsHit(FighterStateType st) {
	return st == FighterStateType::HI_HIT || st == FighterStateType::LO_HIT;
}

bool stateIsDef(FighterStateType st) {
	return st == FighterStateType::HI_DEF || st == FighterStateType::LO_DEF;
}

bool stateIsJmp(FighterStateType st) {
	return st == JMP;
}

Fighter::Fighter(std::string name, FighterSprite *spr, bool onTheLeft) : name(name), sprite(spr), inputController(), stateTransitions(),
	tickAnimator(), frameListAnimator(), frameRangeAnimator(), movingAnimator(), movingPathAnimator(), flashAnimator() {

	currentlyRunningAnimator = &frameRangeAnimator;
	lastHP = HP = 150;
	isOnTheLeft = onTheLeft;
	ignoreInput = true;
	flag = true;
	wins = 0;
	losses = 0;
	if (!isOnTheLeft) {
		getSprite()->flip();
		sprite->setHAlign(HFrameAlign::RIGHT);
	}
}

void Fighter::resetY() {
	sprite->setBorder(3, sprite->getLayer()->height - sprite->getLayer()->border_south);
	sprite->setY(sprite->getBorder(3) - sprite->getFrameBox().h);
}

void Fighter::processReceivedInput(bool useMaximalEdges) {
	if (!ignoreInput) stateTransitions.performTransitions(inputController.getLogical(), useMaximalEdges);
}

InputController& Fighter::getInputController() {
	return inputController;
}

StateTransitions& Fighter::getStateTransitions() {
	return stateTransitions;
}

void Fighter::addMovingAnimation(std::string name, MovingAnimation* ma, MovingAnimation* maInv, const AnimationFilm *film) {
	allMovingAnims[name] = std::tuple<MovingAnimation*, MovingAnimation*, const AnimationFilm*>(ma, maInv, film);
	animationType[name] = "moving";
}

void Fighter::addFrameRangeAnimation(std::string name, FrameRangeAnimation* fa, FrameRangeAnimation* faInv, const AnimationFilm *film) {
	allFrameRangeAnims[name] = std::tuple<FrameRangeAnimation*, FrameRangeAnimation*, const AnimationFilm*>(fa, faInv, film);
	animationType[name] = "frameRange";
}

void Fighter::addFrameListAnimation(std::string name, FrameListAnimation* fla, FrameListAnimation* flaInv, const AnimationFilm *film) {
	allFrameListAnims[name] = std::tuple<FrameListAnimation*, FrameListAnimation*, const AnimationFilm*>(fla, flaInv, film);
	animationType[name] = "frameList";
}

void Fighter::addMovingPathAnimation(std::string name, MovingPathAnimation* mpa, MovingPathAnimation* mpaInv, const AnimationFilm *film) {
	allMovingPathAnims[name] = std::tuple<MovingPathAnimation*, MovingPathAnimation*, const AnimationFilm *>(mpa, mpaInv, film);
	animationType[name] = "movingPath";
}

void Fighter::addFlashAnimation(std::string name, FlashAnimation* fsa, const AnimationFilm *film) {
	allFlashAnims[name] = std::pair<FlashAnimation*, const AnimationFilm *>(fsa, film);
	animationType[name] = "flash";
}

void Fighter::setNextAction(std::string na) {		
	nextAction = na;
}

Animator* Fighter::getCurrentlyRunningAnimator() {
	return currentlyRunningAnimator;
}

const AnimationFilm* Fighter::getMovingPathAnimationFilm(std::string a) {
	return std::get<2>(allMovingPathAnims[a]);
}

std::string Fighter::getNextAction() {	
	return nextAction;
}

FighterSprite* Fighter::getSprite() {
	return sprite;
}

void Fighter::setTickAnimation(TickAnimation* t) {
	tickAnimation = t;
}

void Fighter::startTickAnimator(timestamp_t currTime) {
	tickAnimator.start(tickAnimation, currTime);
	AnimatorHolder::markAsRunning(&tickAnimator);
}

void Fighter::startPlayingAnimation(std::string animationName, timestamp_t currTime) {
	getCurrentlyRunningAnimator()->stop(true);
	AnimatorHolder::markAsSuspended(getCurrentlyRunningAnimator());
	std::string &t = animationType[animationName];
	if (t == "frameRange") {
		sprite->setAnimationFilm(std::get<2>(allFrameRangeAnims[animationName]));
		if(isOnTheLeft)
			frameRangeAnimator.start(sprite, std::get<0>(allFrameRangeAnims[animationName]), currTime);
		else
			frameRangeAnimator.start(sprite, std::get<1>(allFrameRangeAnims[animationName]), currTime);
		AnimatorHolder::markAsRunning(&frameRangeAnimator);
		currentlyRunningAnimator = &frameRangeAnimator;
	}
	else if (t == "frameList") {
		sprite->setAnimationFilm(std::get<2>(allFrameListAnims[animationName]));
		if(isOnTheLeft)
			frameListAnimator.start(sprite, std::get<0>(allFrameListAnims[animationName]), currTime);
		else
			frameListAnimator.start(sprite, std::get<1>(allFrameListAnims[animationName]), currTime);
		AnimatorHolder::markAsRunning(&frameListAnimator);
		currentlyRunningAnimator = &frameListAnimator;
	}
	else if (t == "moving") {
		sprite->setAnimationFilm(std::get<2>(allMovingAnims[animationName]));
		if(isOnTheLeft)
			movingAnimator.start(sprite, std::get<0>(allMovingAnims[animationName]), currTime);
		else
			movingAnimator.start(sprite, std::get<1>(allMovingAnims[animationName]), currTime);
		AnimatorHolder::markAsRunning(&movingAnimator);
		currentlyRunningAnimator = &movingAnimator;
	}
	else if (t == "movingPath") {
		sprite->setAnimationFilm(std::get<2>(allMovingPathAnims[animationName]));
		if(isOnTheLeft)
			movingPathAnimator.start(sprite, std::get<0>(allMovingPathAnims[animationName]), currTime);
		else
			movingPathAnimator.start(sprite, std::get<1>(allMovingPathAnims[animationName]), currTime);
		AnimatorHolder::markAsRunning(&movingPathAnimator);
		currentlyRunningAnimator = &movingPathAnimator;
	}
	else if (t == "flash") {
		sprite->setAnimationFilm(allFlashAnims[animationName].second);
		flashAnimator.start(sprite, allFlashAnims[animationName].first, currTime);
		AnimatorHolder::markAsRunning(&flashAnimator);
		currentlyRunningAnimator = &flashAnimator;
	}
	else {
		assert(false);
	}
	currentlyRunningAnimator->setOnFinish(nullptr);
}

int Fighter::getHP() {
	return HP;
}

int Fighter::getLastHP() {
	return lastHP;
}

void Fighter::setHP(int hp) {
	if (HP < 0) return;
	if (hp >= 0) HP = hp;	
	else HP = 0;
}

void Fighter::setLastHP(int hp) {
	lastHP = hp;
}

void Fighter::flipSide() {
	getInputController().reverseForward();
	getSprite()->flip();
	isOnTheLeft = !isOnTheLeft;
	if(isOnTheLeft)
		sprite->setHAlign(HFrameAlign::LEFT);
	else
		sprite->setHAlign(HFrameAlign::RIGHT);
}

bool Fighter::isOnLeftSide() {
	return isOnTheLeft;
}

std::string Fighter::getState() {
	return stateTransitions.getState();
}

std::string Fighter::getPrevState() {
	return prevState;
}

bool Fighter::inInterruptibleState() {
	std::string state = stateTransitions.getState();
	return (state == "idle" || state == "moveForward" || state == "moveBackwards" || state == "duck") && (nextAction != state);
}

void Fighter::setState(std::string st) {
	auto tmp = getStateType(st);
	if (stateIsNeu(tmp))
		sprite->setState("neu");
	else if (stateIsAtk(tmp))
		sprite->setState("atk");
	else if (stateIsDef(tmp))
		sprite->setState("def");
	else if (stateIsHit(tmp))
		sprite->setState("hit");
	else if (stateIsJmp(tmp))
		sprite->setState("jmp");
	else
		assert(0);
	prevState = getState();
	stateTransitions.setState(st);
}

std::string Fighter::getName() {
	return name;
}

void Fighter::setAnimationFilm(std::string filmName) {
	const AnimationFilm *film;
	std::string animType = animationType[filmName];
	if (animType == "moving")
		film = std::get<2>(allMovingAnims[filmName]);
	else if (animType == "frameRange")
		film = std::get<2>(allFrameRangeAnims[filmName]);
	else if (animType == "frameList")
		film = std::get<2>(allFrameListAnims[filmName]);
	else if (animType == "movingPath")
		film = std::get<2>(allMovingPathAnims[filmName]);
	else if (animType == "flash")
		film = allFlashAnims[filmName].second;
	else {
		film = nullptr;
		assert(0);
	}
	sprite->setAnimationFilm(film);
}

void Fighter::addStateType(std::string state, FighterStateType type) {	
	stateTypes[state] = type;
}

FighterStateType Fighter::getStateType(std::string state) {
	return stateTypes[state];
}

void Fighter::mapStrikeToHitState(std::string strikeName, std::string hitStateName) {
	strikeToHitState[strikeName] = hitStateName;
}

bool Fighter::strikeSuccess(std::string strike, Fighter *other) {
	auto otherStateType = other->stateTypes[other->getState()];
	auto iter = strikeInfo.find(strike);
	//if this is not a strike, or if the other fighter's state category is not in the hittable states
	if (iter == strikeInfo.end() || iter->second.first.find(otherStateType) == iter->second.first.end())
		return false;
	//if this particular state has been registered as excluded from this particular strike
	if (other->strikeToHitState.find(strike + "." + other->getState()) != other->strikeToHitState.end() &&
		other->strikeToHitState[strike + "." + other->getState()] == "exclude")
		return false;
	return true;
}

void Fighter::attack(Fighter *other) {	
	auto otherStateType = other->stateTypes[other->getState()];

	if (strikeSuccess(getState(), other)) {

		if (otherStateType == FighterStateType::LO_DEF) {
			other->setState("hitLowBlock");
			other->setNextAction("hitLowBlock");
			other->applyDamage(strikeInfo[getState()].second * 0.2);
		}
		else if (otherStateType == FighterStateType::HI_DEF) {
			other->setState("hitHighBlock");
			other->setNextAction("hitHighBlock");
			other->applyDamage(strikeInfo[getState()].second * 0.2);
		}
		else {
			//find what state the other fighter goes to after this hit (also depends on his state)
			auto iter = strikeToHitState.find(getState() + "." + other->getState());
			//if no special animation specified for his current state, search for his default one
			if (iter == strikeToHitState.end())
				iter = strikeToHitState.find(getState() + ".any");
			other->setState(iter->second);
			other->setNextAction(iter->second);
			other->applyDamage(strikeInfo[getState()].second);
		}
		if (other->getHP() <= 0) {			
			if (other->getNextAction() != "falling_big" && other->getNextAction() != "hitThrow" && other->getNextAction() != "tripped") {
				other->setState("falling");
				other->setNextAction("falling");
			}
		}
		
		if (other->getPrevState() == "dizzy") other->currentlyRunningAnimator->stop(false);
		else other->currentlyRunningAnimator->stop(true);
	}
}

void Fighter::applyDamage(int dmg) {	
	setHP(getHP() - dmg);
}

void Fighter::setNextToOther(bool proximity) {
	nextToOther = proximity;
}

bool Fighter::isNextToOther() {
	return nextToOther;
}

void Fighter::setStrikeDamage(std::string strike, int dmg) {
	strikeInfo[strike].second = dmg;
}

void Fighter::addStrikeCapability(std::string strike, FighterStateType type) {
	strikeInfo[strike].first.insert(type);
}

Projectile Fighter::createProjectile(std::string projectile, timestamp_t currTime) {
	Sprite *projectileSpr = nullptr;
	int projXleft = sprite->getX() + initProjectileOffsetLeft < 0 ? 0 : sprite->getX() + initProjectileOffsetLeft;
	Dim createX = (isOnTheLeft) ? (projXleft) : (sprite->getX() - initProjectileOffsetRight);
	Dim createY = sprite->getY() + initProjectileOffsetY;
	const AnimationFilm *projectileFilm;
	auto align = (isOnTheLeft) ? (HFrameAlign::LEFT) : (HFrameAlign::RIGHT);
	auto tmp = allMovingPathAnims[projectile];
	projectileFilm = std::get<2>(tmp);
	auto anim = (isOnTheLeft) ? (std::get<0>(tmp)) : (std::get<1>(tmp));
	projectileSpr = new Sprite(createX, createY, sprite->getZOrder(),
		sprite->getLayer(), projectileFilm, align, VFrameAlign::BOTTOM, projectileName, "noState");
	SpritesHolder s = SpritesHolder::Get();
	s.Add(projectileSpr);
	MovingPathAnimator *projectileAnimator = new MovingPathAnimator();
	projectileAnimator->start(projectileSpr, anim, currTime);
	AnimatorHolder::markAsRunning(projectileAnimator);
	if (!isOnTheLeft)
		projectileSpr->flip();
	return Projectile({ projectileSpr, projectileAnimator, projectileDmg });
}

Projectile Fighter::createProjectile2(std::string projectile, timestamp_t currTime) {
	Sprite *projectileSpr = nullptr;
	int projXleft = sprite->getX() + initProjectile2OffsetLeft < 0 ? 0 : sprite->getX() + initProjectile2OffsetLeft;
	Dim createX = (isOnTheLeft) ? (projXleft) : (sprite->getX() - initProjectile2OffsetRight);
	Dim createY = sprite->getY() + initProjectile2OffsetY;
	const AnimationFilm *projectileFilm;
	auto align = (isOnTheLeft) ? (HFrameAlign::LEFT) : (HFrameAlign::RIGHT);
	auto tmp = allMovingPathAnims[projectile];
	projectileFilm = std::get<2>(tmp);
	auto anim = (isOnTheLeft) ? (std::get<0>(tmp)) : (std::get<1>(tmp));
	projectileSpr = new Sprite(createX, createY, sprite->getZOrder(),
		sprite->getLayer(), projectileFilm, align, VFrameAlign::BOTTOM, projectileName, "noState");
	SpritesHolder s = SpritesHolder::Get();
	s.Add(projectileSpr);
	MovingPathAnimator *projectileAnimator = new MovingPathAnimator();
	projectileAnimator->start(projectileSpr, anim, currTime);
	AnimatorHolder::markAsRunning(projectileAnimator);
	if (!isOnTheLeft)
		projectileSpr->flip();
	return Projectile({ projectileSpr, projectileAnimator, projectileDmg });
}

int Fighter::getWins() {
	return wins;
}

void Fighter::incWins() {
	wins++;
}

void Fighter::setProjectileStats(std::string name, int dmg, Dim leftOffset, Dim rightOffset, Dim yOffset) {
	initProjectileOffsetLeft = leftOffset;
	initProjectileOffsetRight = rightOffset;
	initProjectileOffsetY = yOffset;
	projectileDmg = dmg;
	projectileName = name;
}

void Fighter::setProjectile2Stats(std::string name, int dmg, Dim leftOffset, Dim rightOffset, Dim yOffset) {
	initProjectile2OffsetLeft = leftOffset;
	initProjectile2OffsetRight = rightOffset;
	initProjectile2OffsetY = yOffset;	
	projectile2Dmg = dmg;
	projectile2Name = name;
}

void Fighter::setIgnoreInput(bool flag) {
	ignoreInput = flag;
}

int Fighter::getLosses() {
	return losses;
}

void Fighter::incLosses() {
	losses++;
}

void Fighter::setFlag(bool flag) {
	flag = flag;
}

bool Fighter::getFlag() {
	return flag;
}

void Fighter::setHPforcefully(int hp) {
	HP = hp;
}