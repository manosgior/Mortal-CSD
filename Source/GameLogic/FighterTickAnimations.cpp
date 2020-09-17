#include "FighterTickAnimations.h"
#include "../Engine/Sprite.h"
#include "../Engine/AnimationsHolder.h"
#include "../Engine/AnimationFilmHolder.h"
#include "../Engine/SpritesHolder.h"
#include "../Engine/CollisionChecker.h"
#include "FighterCollisions.h"
#include <iostream>

//------------idle------------

void onIdle(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("idle");
	if (fighter->getState() == "duck") {
		fighter->startPlayingAnimation("duckToIdle", *currTime);
	}
	else if (fighter->getState() == "highBlock") {
		fighter->startPlayingAnimation("highBlockToIdle", *currTime);
	}
	else
		fighter->startPlayingAnimation("idle", *currTime);
}

//------------moves------------

void onMoveForward(Fighter *fighter, timestamp_t *currTime) {
	if (fighter->getState() != "moveForward") {
		fighter->setState("moveForward");
		fighter->startPlayingAnimation("moveForward", *currTime);
	}
}

void onMoveBackwards(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("moveBackwards");
	fighter->startPlayingAnimation("moveBackwards", *currTime);
}

void onDuck(Fighter *fighter, timestamp_t *currTime) {
	if (fighter->getState() == "lowToHighKick" || fighter->getState() == "lowBlock" || fighter->getState() == "duck") {
		fighter->setState("duck");
		fighter->setAnimationFilm("duck");
		fighter->getSprite()->setFrame(2);
	}
	else if (fighter->getState() != "duck") {
		fighter->setState("duck");
		fighter->startPlayingAnimation("duck", *currTime);
	}
}

void onJump(Fighter *fighter, timestamp_t *currTime) {
	AudioHolder::playSound("jump");
	fighter->setState("jump");
	fighter->startPlayingAnimation("jump", *currTime);
}

void onFinishFlip(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("idle");
	fighter->setNextAction("idle");
	fighter->flipSide();
}

void onFlipLeft(Fighter *fighter, timestamp_t *currTime) {
	fighter->startPlayingAnimation("flipToLeft", *currTime);
	fighter->setOnFinish(std::bind(onFinishFlip, fighter, currTime));
}

void onFlipRight(Fighter *fighter, timestamp_t *currTime) {
	fighter->startPlayingAnimation("flipToLeft", *currTime);
	fighter->setOnFinish(std::bind(onFinishFlip, fighter, currTime));
}

//------------kicks------------

void onHighKick(Fighter *fighter, timestamp_t *currTime) {
	AudioHolder::playSound("uppercut");
	if (fighter->isNextToOther()) {
		fighter->setState("kneeKick");
		fighter->getSprite()->setCollisionOffset(-40);
		fighter->startPlayingAnimation("kneeKick", *currTime);
	}
	else {
		fighter->setState("highKick");
		fighter->startPlayingAnimation("highKick", *currTime);
	}
}

void onLowKick(Fighter *fighter, timestamp_t *currTime) {
	AudioHolder::playSound("uppercut");
	if (fighter->isNextToOther()) {
		fighter->setState("kneeKick");
		fighter->startPlayingAnimation("kneeKick", *currTime);
	}
	else {
		fighter->setState("lowKick");
		fighter->startPlayingAnimation("lowKick", *currTime);
	}
}

void afterSweepKick(Fighter *fighter, Dim b) {
	fighter->getSprite()->setBorder(3, b);
}

void onSweepKick(Fighter *fighter, timestamp_t *currTime) {	
	Dim b = fighter->getSprite()->getBorder(3);
	fighter->getSprite()->setBorder(3, b + 20);
	fighter->setState("sweepKick");
	fighter->startPlayingAnimation("sweepKick", *currTime);
	AudioHolder::playSound("uppercut");
	fighter->setOnFinish(std::bind(afterSweepKick, fighter, b));
}

void onRoundhouseKick(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("roundhouseKick");
	AudioHolder::playSound("uppercut");
	fighter->startPlayingAnimation("roundhouseKick", *currTime);
}

void onLowToHighKick(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("lowToHighKick");
	fighter->startPlayingAnimation("lowToHighKick", *currTime);
}

//------------blocks------------

void onHighBlock(Fighter *fighter, timestamp_t *currTime) {	
	if (fighter->getState() != "highBlock") {
		fighter->setState("highBlock");
		fighter->startPlayingAnimation("highBlock", *currTime);
	}		
}

void onLowBlock(Fighter *fighter, timestamp_t *currTime) {	
	if (fighter->getState() != "lowBlock") {
		fighter->setState("lowBlock");
		fighter->startPlayingAnimation("lowBlock", *currTime);
	}
}

//------------punches------------

void onUppercut(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("uppercut");
	AudioHolder::playSound("uppercut");
	fighter->startPlayingAnimation("uppercut", *currTime);
}

void onHighPunch(Fighter *fighter, timestamp_t *currTime) {
	if (fighter->getState() != "highPunch1") {
		if (fighter->isNextToOther() && fighter->getState() != "highPunch2") {
			fighter->getSprite()->setCollisionOffset(-10);
			fighter->setState("elbowStrike");
			fighter->startPlayingAnimation("elbowStrike", *currTime);
		}
		else {
			AudioHolder::playSound("highPunch");
			AudioHolder::playSound("punches");
			fighter->setState("highPunch1");
			fighter->startPlayingAnimation("highPunch1", *currTime);
		}
	}
	else {
		AudioHolder::playSound("punches");
		fighter->setState("highPunch2");
		fighter->startPlayingAnimation("highPunch2", *currTime);
	}
}

void onLowPunch(Fighter *fighter, timestamp_t *currTime) {
	if (fighter->getState() != "lowPunch1") {
		if (fighter->isNextToOther() && fighter->getState() != "lowPunch2") {
			fighter->getSprite()->setCollisionOffset(-10);
			fighter->setState("throw");
			fighter->startPlayingAnimation("throw", *currTime);
		}
		else {
			AudioHolder::playSound("highPunch");
			AudioHolder::playSound("punches");
			fighter->setState("lowPunch1");
			fighter->startPlayingAnimation("lowPunch1", *currTime);
		}
	}
	else {
		AudioHolder::playSound("punches");
		fighter->setState("lowPunch2");
		fighter->startPlayingAnimation("lowPunch2", *currTime);
	}
}

//-------------jumps--------------

void onRightJump(Fighter *fighter, timestamp_t *currTime) {
	AudioHolder::playSound("jump");
	fighter->setState("rightJump");
	fighter->startPlayingAnimation("rightJump", *currTime);
}

void onLeftJump(Fighter *fighter, timestamp_t *currTime) {
	AudioHolder::playSound("jump");
	fighter->setState("leftJump");
	fighter->startPlayingAnimation("leftJump", *currTime);
}

void onForwardFlyKick(Fighter *fighter, timestamp_t *currTime) {
	AudioHolder::playSound("uppercut");
	MovingPathAnimator *anim = (MovingPathAnimator *)fighter->getCurrentlyRunningAnimator();
	int difference = anim->getMaxIndex() - anim->getPathIndex();
	if (anim->getState() != ANIMATOR_RUNNING || difference < 6) {
		return;
	}
	int curPathInd = ((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->getPathIndex();
	timestamp_t lt = fighter->getCurrentlyRunningAnimator()->getLastTime();
	fighter->setState("forwardFlyKick");
	fighter->startPlayingAnimation("forwardFlyKick", fighter->getCurrentlyRunningAnimator()->getStartingTime());
	fighter->getCurrentlyRunningAnimator()->setLastTime(lt);
	std::vector<int> frames = std::vector<int>();
	frames.push_back(0);
	frames.push_back(1);
	frames.push_back(1);
	for (int i = 0; i < difference - 5; i++) frames.push_back(2);	
	frames.push_back(0);
	frames.push_back(3);


	((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->setPathIndex(curPathInd);
	((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->getAnimation()->calibratePathEntryFrames(curPathInd, frames);
}

void onBackwardFlyKick(Fighter *fighter, timestamp_t *currTime) {
	AudioHolder::playSound("uppercut");
	MovingPathAnimator *anim = (MovingPathAnimator *)fighter->getCurrentlyRunningAnimator();
	int difference = anim->getMaxIndex() - anim->getPathIndex();
	if (anim->getState() != ANIMATOR_RUNNING ||  difference < 6) {
		return;
	}
	int curPathInd = ((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->getPathIndex();
	timestamp_t lt = fighter->getCurrentlyRunningAnimator()->getLastTime();
	fighter->setState("backwardFlyKick");
	fighter->startPlayingAnimation("backwardFlyKick", fighter->getCurrentlyRunningAnimator()->getStartingTime());
	fighter->getCurrentlyRunningAnimator()->setLastTime(lt);
	std::vector<int> frames = std::vector<int>();
	frames.push_back(0);
	frames.push_back(1);
	frames.push_back(1);
	for (int i = 0; i < difference - 5; i++) frames.push_back(2);
	frames.push_back(0);
	frames.push_back(3);
	((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->setPathIndex(curPathInd);
	((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->getAnimation()->calibratePathEntryFrames(curPathInd, frames);
}

void onForwardFlyPunch(Fighter *fighter, timestamp_t *currTime) {
	AudioHolder::playSound("uppercut");
	MovingPathAnimator *anim = (MovingPathAnimator *)fighter->getCurrentlyRunningAnimator();
	int difference = anim->getMaxIndex() - anim->getPathIndex();
	if (anim->getState() != ANIMATOR_RUNNING || difference < 7) {
		return;
	}
	int curPathInd = ((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->getPathIndex();
	timestamp_t lt = fighter->getCurrentlyRunningAnimator()->getLastTime();
	fighter->setState("forwardFlyPunch");
	fighter->startPlayingAnimation("forwardFlyPunch", fighter->getCurrentlyRunningAnimator()->getStartingTime());
	fighter->getCurrentlyRunningAnimator()->setLastTime(lt);
	std::vector<int> frames = std::vector<int>();

	frames.push_back(0);
	frames.push_back(1); 	
	frames.push_back(1);
	for (int i = 0; i < difference - 5; i++) frames.push_back(2);
	frames.push_back(0);
	frames.push_back(0);
	frames.push_back(0);
	frames.push_back(3);

	((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->setPathIndex(curPathInd);
	((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->getAnimation()->calibratePathEntryFrames(curPathInd, frames);
}

void onBackwardFlyPunch(Fighter *fighter, timestamp_t *currTime) {
	AudioHolder::playSound("uppercut");
	MovingPathAnimator *anim = (MovingPathAnimator *)fighter->getCurrentlyRunningAnimator();
	int difference = anim->getMaxIndex() - anim->getPathIndex();
	if (anim->getState() != ANIMATOR_RUNNING || difference < 7) {
		return;
	}
	int curPathInd = ((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->getPathIndex();
	timestamp_t lt = fighter->getCurrentlyRunningAnimator()->getLastTime();
	fighter->setState("backwardFlyPunch");
	fighter->startPlayingAnimation("backwardFlyPunch", fighter->getCurrentlyRunningAnimator()->getStartingTime());
	fighter->getCurrentlyRunningAnimator()->setLastTime(lt);
	std::vector<int> frames = std::vector<int>();
	frames.push_back(0);
	frames.push_back(1);
	frames.push_back(1);
	for (int i = 0; i < difference - 5; i++) frames.push_back(2);
	frames.push_back(0);
	frames.push_back(0);
	frames.push_back(0);
	frames.push_back(3);
	((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->setPathIndex(curPathInd);
	((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->getAnimation()->calibratePathEntryFrames(curPathInd, frames);
}

void onStationaryFlyPunch(Fighter *fighter, timestamp_t *currTime) {
	AudioHolder::playSound("uppercut");
	MovingPathAnimator *anim = (MovingPathAnimator *)fighter->getCurrentlyRunningAnimator();
	int difference = anim->getMaxIndex() - anim->getPathIndex();
	if (anim->getState() != ANIMATOR_RUNNING || difference < 7) {
		return;
	}
	int curPathInd = ((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->getPathIndex();
	timestamp_t lt = fighter->getCurrentlyRunningAnimator()->getLastTime();
	fighter->setState("stationaryFlyPunch");
	fighter->startPlayingAnimation("stationaryFlyPunch", fighter->getCurrentlyRunningAnimator()->getStartingTime());
	fighter->getCurrentlyRunningAnimator()->setLastTime(lt);
	std::vector<int> frames = std::vector<int>();
	
	frames.push_back(0);
	frames.push_back(1); 
	difference -= 2;
	for (int i = 0; i < difference / 2; i++) frames.push_back(2);
	for (int i = 0; i < difference / 2; i++) frames.push_back(0);	
	frames.push_back(3);

	((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->setPathIndex(curPathInd);
	((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->getAnimation()->calibratePathEntryFrames(curPathInd, frames);
}

void onStationaryFlyKick(Fighter *fighter, timestamp_t *currTime) {
	AudioHolder::playSound("uppercut");
	MovingPathAnimator *anim = (MovingPathAnimator *)fighter->getCurrentlyRunningAnimator();
	int difference = anim->getMaxIndex() - anim->getPathIndex();
	if (anim->getState() != ANIMATOR_RUNNING || difference < 7) {
		return;
	}
	int curPathInd = ((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->getPathIndex();
	timestamp_t lt = fighter->getCurrentlyRunningAnimator()->getLastTime();
	fighter->setState("stationaryFlyKick");
	fighter->startPlayingAnimation("stationaryFlyKick", fighter->getCurrentlyRunningAnimator()->getStartingTime());
	fighter->getCurrentlyRunningAnimator()->setLastTime(lt);
	std::vector<int> frames = std::vector<int>();
	
	for (int i = 0; i < difference / 2; i++) frames.push_back(0);
	for (int i = 0; i < difference / 2; i++) frames.push_back(1);	
	frames.push_back(2);
	
	((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->setPathIndex(curPathInd);
	((MovingPathAnimator *)fighter->getCurrentlyRunningAnimator())->getAnimation()->calibratePathEntryFrames(curPathInd, frames);
}

//-------------hit animations--------------

void returnToIdle(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("idle");
	fighter->setNextAction("idle");
}

void gettingUp(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("gettingUp");
	fighter->resetY();
	fighter->setNextAction("gettingUp");
	AudioHolder::playSound("afterFall");
}

void dead(Fighter *fighter) {
	fighter->setNextAction("dead");
	fighter->setState("dead");
}

void returnToDuck(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("duck");
	fighter->setNextAction("duck");
}

void onFalling(Fighter *fighter, timestamp_t *currTime) {	
	fighter->startPlayingAnimation("falling", *currTime);
	AudioHolder::playSound("fallingBig");
	if (fighter->getHP() > 0 || (fighter->getHP() == 0 && fighter->getLosses() == 1)) fighter->setOnFinish(std::bind(gettingUp, fighter, currTime));		
	else fighter->setOnFinish(std::bind(dead, fighter));
}

void deadTripped(Fighter *fighter) {
	fighter->setNextAction("deadTripped");
	fighter->setState("deadTripped");
}

void onDead(Fighter *fighter, timestamp_t *currTime) {
	fighter->startPlayingAnimation("dead", *currTime);
	fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));
}

void onDeadTripped(Fighter *fighter, timestamp_t *currTime) {
	fighter->startPlayingAnimation("deadTripped", *currTime);
	//fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));
}

void falling(Fighter *fighter) {
	fighter->setNextAction("falling");
	fighter->setState("falling");
}

void dizzy(Fighter *fighter) {
	fighter->setState("dizzy");
	fighter->setNextAction("dizzy");
}

void onDizzy(Fighter *fighter, timestamp_t *currTime) {	
	fighter->startPlayingAnimation("dizzy", *currTime);
	if (fighter->getHP() == 0 && fighter->getLosses() == 1) {
		fighter->setHPforcefully(-1);
		fighter->setLastHP(-1);
		fighter->setOnFinish(std::bind(falling, fighter));
	}
	else if (fighter->getHP() == 0) {
		fighter->setOnFinish(std::bind(falling, fighter));
	}
	else fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));
}

void onHitHigh(Fighter *fighter, timestamp_t *currTime) { 
	fighter->startPlayingAnimation("hitHigh", *currTime);
	fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));
	AudioHolder::playSound("hitHigh");
}

void onHitLowWhenHigh(Fighter *fighter, timestamp_t *currTime) {	
	fighter->startPlayingAnimation("hitLowWhenHigh", *currTime);
	fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));	
	AudioHolder::playSound("hitHigh");
}

void onHitLowWhenHighStatic(Fighter *fighter, timestamp_t *currTime) {	
	fighter->startPlayingAnimation("hitLowWhenHighStatic", *currTime);
	fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));	
	AudioHolder::playSound("hitLow");
}

void onHitLow(Fighter *fighter, timestamp_t *currTime) {	
	fighter->startPlayingAnimation("hitLow", *currTime);
	fighter->setOnFinish(std::bind(returnToDuck, fighter, currTime));	
	AudioHolder::playSound("hitLow");
}

void onStagger(Fighter *fighter, timestamp_t *currTime) {	
	fighter->startPlayingAnimation("stagger", *currTime);
	fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));	
	AudioHolder::playSound("hitDefault1");
}

void onFallingBig(Fighter *fighter, timestamp_t *currTime) {	
	fighter->startPlayingAnimation("falling_big", *currTime);
	AudioHolder::playSound("fallingBig");
	AudioHolder::playSound("wow");
	if (fighter->getHP() > 0 || (fighter->getHP() == 0 && fighter->getLosses() == 1)) fighter->setOnFinish(std::bind(gettingUp, fighter, currTime));
	else fighter->setOnFinish(std::bind(dead, fighter));	
}

void onHitThrow(Fighter *fighter, timestamp_t *currTime) {
	fighter->startPlayingAnimation("hitThrow", *currTime);
	AudioHolder::playSound("hitThrow");

	if (fighter->getHP() > 0 || (fighter->getHP() == 0 && fighter->getLosses() == 1)) fighter->setOnFinish(std::bind(gettingUp, fighter, currTime));	
	else fighter->setOnFinish(std::bind(dead, fighter));
}

void onTripped(Fighter *fighter, timestamp_t *currTime) {
	fighter->startPlayingAnimation("tripped", *currTime);
	AudioHolder::playSound("hitDefault2");
	if (fighter->getHP() > 0 || (fighter->getHP() == 0 && fighter->getLosses() == 1)) fighter->setOnFinish(std::bind(gettingUp, fighter, currTime));	
	else fighter->setOnFinish(std::bind(deadTripped, fighter));
}

void onHitHighBlock(Fighter *fighter, timestamp_t *currTime) {	
	fighter->startPlayingAnimation("hitHighBlock", *currTime);
	fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));	
}

void onHitLowBlock(Fighter *fighter, timestamp_t *currTime) {	
	fighter->startPlayingAnimation("hitLowBlock", *currTime);
	fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));
}

void onGettingUp(Fighter *fighter, timestamp_t *currTime) {	
	fighter->startPlayingAnimation("gettingUp", *currTime); 
	if (fighter->getHP() == 0 && fighter->getLosses() == 1) fighter->setOnFinish(std::bind(dizzy, fighter));
	else fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));	
}

void onSpearRelease(Fighter *fighter, timestamp_t *currTime) {

}

void onVictory(Fighter *fighter, timestamp_t *currTime) {
	fighter->startPlayingAnimation("victory", *currTime);
	fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));
}

//-------------specials--------------

void onThrowIce(Fighter *fighter, Fighter *otherFighter, timestamp_t *currTime) {
	fighter->setState("throwIce");
	fighter->startPlayingAnimation("throwIce", *currTime);
	Projectile p = fighter->createProjectile("projectile", *currTime);
	p.animator->setOnFinish(std::bind(destroyProjectile, p.spr, otherFighter->getSprite()));
	auto col = IceCollisionHandler();
	col.f = otherFighter;
	col.p = p;
	col.currTime = currTime;
	otherFighter->getSprite()->addCollisionHandler(col);
	CollisionChecker::Get().registerPair(otherFighter->getSprite(), p.spr, true);

}

void onFrozen(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("frozen");
	fighter->setNextAction("frozen");
}

void onFrozenOnAir(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("frozenOnAir");
	fighter->setNextAction("frozenOnAir");
}

void onImpaled(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("impaled");
	fighter->setNextAction("impaled");
}

void onImpaledMove(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("impaledMove");
	fighter->setNextAction("impaledMove");
}

void onSlide(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("slide");
	fighter->startPlayingAnimation("subzero_slide", *currTime);
	fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));
}

void onDragSpear(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("dragSpear");
	fighter->setNextAction("dragSpear");
}

void destroySpear(Sprite *pspr, Fighter *wielder, Fighter *other, timestamp_t *currTime) {
	destroyProjectile(pspr, other->getSprite());
	returnToIdle(wielder, currTime);
}

void onThrowSpear(Fighter *fighter, Fighter *otherFighter, timestamp_t *currTime) {
	fighter->setState("throwSpear");
	fighter->setNextAction("throwSpear");
	fighter->startPlayingAnimation("scorpion_throwSpear", *currTime);
	AudioHolder::playSound("throwSpear");
	Projectile p = fighter->createProjectile("projectile", *currTime);
	p.animator->setOnFinish(std::bind(destroySpear, p.spr, fighter, otherFighter, currTime));
	auto col = SpearCollisionHandler();
	col.f = otherFighter;
	col.wielder = fighter;
	col.p = p;
	col.currTime = currTime;
	otherFighter->getSprite()->addCollisionHandler(col);
	CollisionChecker::Get().registerPair(otherFighter->getSprite(), p.spr, true);
}

void onTeleportPunchAttack(Fighter *fighter, timestamp_t *currTime, ViewWindow *viewWindow) {
	fighter->setState("teleportPunchAttack");
	if (fighter->isOnLeftSide() && std::abs(fighter->getSprite()->getX() - viewWindow->getX()) < 30) {
		fighter->getSprite()->move(0, -80);
		fighter->getSprite()->setX(viewWindow->getX() + viewWindow->getWidth() - 60);
		fighter->startPlayingAnimation("teleportPunchAttack", *currTime);
		fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));
	}
	else if (!fighter->isOnLeftSide() && std::abs(viewWindow->getX() + viewWindow->getWidth() - 
			fighter->getSprite()->getX() - fighter->getSprite()->getFrameBox().w) < 30) {
		fighter->getSprite()->move(0, -80);
		fighter->getSprite()->setX(viewWindow->getX() + 60);
		fighter->startPlayingAnimation("teleportPunchAttack", *currTime);
		fighter->setOnFinish(std::bind(returnToIdle, fighter, currTime));
	}
	else {
		returnToIdle(fighter, currTime);
	}
}

void onDeadBurned(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("deadBurned");
	fighter->startPlayingAnimation("deadBurned", *currTime);
}

void onTeleportPunch(Fighter *fighter, timestamp_t *currTime) {
	AudioHolder::playSound("jump");
	fighter->setState("teleportPunch");
	fighter->startPlayingAnimation("teleportPunch", *currTime);
}

void onBurned(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("burned");
	AudioHolder::playSound("dead");
	AudioHolder::playSound("fatality");
	fighter->startPlayingAnimation("burned", *currTime);
	fighter->setOnFinish(std::bind(&Fighter::setNextAction, fighter, "deadBurned"));
}

void onTeleportPunchPrep(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("teleportPunchPrep");
	fighter->startPlayingAnimation("duck", *currTime);
}


void onScorpionFatality(Fighter *fighter, Fighter *other, timestamp_t *currTime) {
	Sprite *s1 = fighter->getSprite(), *s2 = other->getSprite();
	int distance = std::abs(s1->getX() - s2->getX());

	if (other->getHP() != -1 || (distance > s1->getFrameBox().w + 100 && distance < s1->getFrameBox().w + 85)) {
		fighter->setNextAction("idle");
	}
	else {
		fighter->setState("scorpionFatality");
		fighter->startPlayingAnimation(fighter->getName() + "_fatality", *currTime);
		
		Projectile p = fighter->createProjectile2("projectileFatality", *currTime);
		p.animator->setOnFinish(std::bind(&Sprite::setVisibility, p.spr, false));	
		other->getCurrentlyRunningAnimator()->stop(false);
		AudioHolder::playSound("fireball");
		other->setNextAction("burned");		
	}	
}

void onDeadDecapitated(Fighter *fighter, timestamp_t *currTime) {	
	fighter->setState("deadDecapitated");
	fighter->startPlayingAnimation("deadDecapitated", *currTime);
}

void onDecapitated(Fighter *fighter, timestamp_t *currTime) {
	fighter->setState("decapitated");
	AudioHolder::playSound("fatality");
	AudioHolder::playSound("dead");
	fighter->startPlayingAnimation("decapitated", *currTime);
	fighter->setOnFinish(std::bind(&Fighter::setNextAction, fighter, "deadDecapitated"));
}

void destroyHead(MovingPathAnimator *headRoller, Sprite *head) {
	head->setVisibility(false);
	auto h =SpritesHolder::Get();
	h.Remove(head);
	AnimatorHolder::cancel(headRoller);
}
void onSubzeroFatality(Fighter *fighter, Fighter *other, timestamp_t *currTime) {
	Sprite *s1 = fighter->getSprite(), *s2 = other->getSprite();
	int distance = std::abs(s1->getX() - s2->getX());

	if (other->getHP() != -1 || (distance > s1->getFrameBox().w + 50)) {
		fighter->setNextAction("idle");
	}
	else {
		AnimationFilmHolder &filmHolder = AnimationFilmHolder::Get();
		SpritesHolder h = SpritesHolder::Get();
		AnimatorHolder animatorHolder;

		fighter->setNextAction("uppercut");
		fighter->setState("subzeroFatality");
		fighter->startPlayingAnimation("uppercut", *currTime);
		
		Sprite *head = new Sprite(s2->getX(), s2->getY(), s2->getZOrder(), s2->getLayer(), filmHolder.GetFilm(other->getName() + "_headRolling"), HFrameAlign::LEFT, VFrameAlign::TOP, "they see me rollin", "the hatin");
		h.Add(head);
		MovingPathAnimation *headRolling = AnimationsHolder::getMovingPathAnim("headRolling");
		MovingPathAnimator *headRoller = new MovingPathAnimator();

		animatorHolder.Register(headRoller);
		headRoller->start(head, headRolling, *currTime);
		headRoller->setOnFinish(std::bind(destroyHead, headRoller, head));
		animatorHolder.markAsRunning(headRoller);
		
		other->getCurrentlyRunningAnimator()->stop(false);
		other->setNextAction("decapitated");	
	}
}

void FighterTickAnimations::basicSetup() {
	actionDispatcher["lowKick"] = std::bind(onLowKick, fighter, currTime);
	actionDispatcher["highKick"] = std::bind(onHighKick, fighter, currTime);
	actionDispatcher["roundhouseKick"] = std::bind(onRoundhouseKick, fighter, currTime);
	actionDispatcher["sweepKick"] = std::bind(onSweepKick, fighter, currTime);
	actionDispatcher["lowToHighKick"] = std::bind(onLowToHighKick, fighter, currTime);

	actionDispatcher["highPunch1"] = std::bind(onHighPunch, fighter, currTime);
	actionDispatcher["highPunch2"] = std::bind(onHighPunch, fighter, currTime);
	actionDispatcher["lowPunch1"] = std::bind(onLowPunch, fighter, currTime);
	actionDispatcher["lowPunch2"] = std::bind(onLowPunch, fighter, currTime);
	actionDispatcher["uppercut"] = std::bind(onUppercut, fighter, currTime);

	actionDispatcher["idle"] = std::bind(onIdle, fighter, currTime);

	actionDispatcher["moveForward"] = std::bind(onMoveForward, fighter, currTime);
	actionDispatcher["moveBackwards"] = std::bind(onMoveBackwards, fighter, currTime);
	actionDispatcher["duck"] = std::bind(onDuck, fighter, currTime);

	actionDispatcher["highBlock"] = std::bind(onHighBlock, fighter, currTime);
	actionDispatcher["lowBlock"] = std::bind(onLowBlock, fighter, currTime);

	actionDispatcher["rightJump"] = std::bind(onRightJump, fighter, currTime);
	actionDispatcher["leftJump"] = std::bind(onLeftJump, fighter, currTime);
	actionDispatcher["forwardFlyKick"] = std::bind(onForwardFlyKick, fighter, currTime);
	actionDispatcher["backwardFlyKick"] = std::bind(onBackwardFlyKick, fighter, currTime);
	actionDispatcher["forwardFlyPunch"] = std::bind(onForwardFlyPunch, fighter, currTime);
	actionDispatcher["backwardFlyPunch"] = std::bind(onBackwardFlyPunch, fighter, currTime);
	actionDispatcher["stationaryFlyPunch"] = std::bind(onStationaryFlyPunch, fighter, currTime);
	actionDispatcher["stationaryFlyKick"] = std::bind(onStationaryFlyKick, fighter, currTime);
	
	actionDispatcher["jump"] = std::bind(onJump, fighter, currTime);

	actionDispatcher["dizzy"] = std::bind(onDizzy, fighter, currTime);
	actionDispatcher["hitHigh"] = std::bind(onHitHigh, fighter, currTime);
	actionDispatcher["hitLow"] = std::bind(onHitLow, fighter, currTime);
	actionDispatcher["hitLowWhenHigh"] = std::bind(onHitLowWhenHigh, fighter, currTime); 
	actionDispatcher["hitLowWhenHighStatic"] = std::bind(onHitLowWhenHighStatic, fighter, currTime);
	actionDispatcher["stagger"] = std::bind(onStagger, fighter, currTime); 
	actionDispatcher["falling"] = std::bind(onFalling, fighter, currTime);		
	actionDispatcher["dead"] = std::bind(onDead, fighter, currTime);
	actionDispatcher["deadTripped"] = std::bind(onDeadTripped, fighter, currTime);
	actionDispatcher["falling_big"] = std::bind(onFallingBig, fighter, currTime);
	actionDispatcher["hitThrow"] = std::bind(onHitThrow, fighter, currTime);
	actionDispatcher["tripped"] = std::bind(onTripped, fighter, currTime);
	actionDispatcher["hitHighBlock"] = std::bind(onHitHighBlock, fighter, currTime);
	actionDispatcher["hitLowBlock"] = std::bind(onHitLowBlock, fighter, currTime);
	actionDispatcher["frozen"] = std::bind(onFrozen, fighter, currTime);
	actionDispatcher["impaled"] = std::bind(onImpaled, fighter, currTime);
	actionDispatcher["impaledMove"] = std::bind(onImpaledMove, fighter, currTime);
	actionDispatcher["gettingUp"] = std::bind(onGettingUp, fighter, currTime);
	actionDispatcher["burned"] = std::bind(onBurned, fighter, currTime);
	actionDispatcher["deadBurned"] = std::bind(onDeadBurned, fighter, currTime);
	actionDispatcher["decapitated"] = std::bind(onDecapitated, fighter, currTime);
	actionDispatcher["deadDecapitated"] = std::bind(onDeadDecapitated, fighter, currTime);

	actionDispatcher["flipToLeft"] = std::bind(onFlipLeft, fighter, currTime);
	actionDispatcher["flipToRight"] = std::bind(onFlipRight, fighter, currTime);
	actionDispatcher["scorpionFatality"] = std::bind(onScorpionFatality, fighter, otherFighter, currTime);
	actionDispatcher["subzeroFatality"] = std::bind(onSubzeroFatality, fighter, otherFighter, currTime);
	actionDispatcher["victory"] = std::bind(onVictory, fighter, currTime);

}

void FighterTickAnimations::setUpSubzeroTickAnimation() {
	basicSetup();

	actionDispatcher["slide"] = std::bind(onSlide, fighter, currTime);
	actionDispatcher["throwIce"] = std::bind(onThrowIce, fighter, otherFighter, currTime);
}

void FighterTickAnimations::setUpScorpionTickAnimation() {
	basicSetup();

	actionDispatcher["throwSpear"] = std::bind(onThrowSpear, fighter, otherFighter, currTime);
	actionDispatcher["dragSpear"] = std::bind(onDragSpear, fighter, currTime);
	actionDispatcher["teleportPunchPrep"] = std::bind(onTeleportPunchPrep, fighter, currTime);
	actionDispatcher["teleportPunch"] = std::bind(onTeleportPunch, fighter, currTime);
	actionDispatcher["teleportPunchAttack"] = std::bind(onTeleportPunchAttack, fighter, currTime, viewWindow);
}

void FighterTickAnimations::onTickFunction() {
	Sprite *s1 = fighter->getSprite(), *s2 = otherFighter->getSprite();
	std::string stateFighter = fighter->getState();
	//flip checks
	if (fighter->isOnLeftSide() && s1->getX() > s2->getX() && stateFighter != "victory" && stateFighter != "dead" && stateFighter != "teleportPunchAttack") {
		if(fighter->getPrevState() == "gettingUp" || stateFighter == "idle" || stateFighter == "throw")
			fighter->setNextAction("flipToLeft");
		else 
			fighter->flipSide();
	}
	else if (!fighter->isOnLeftSide() && s1->getX() < s2->getX() && stateFighter != "victory" && stateFighter != "dead" && stateFighter != "teleportPunchAttack") {
		if (fighter->getPrevState() == "gettingUp" || stateFighter == "idle" || stateFighter == "throw" )
			fighter->setNextAction("flipToRight");
		else
			fighter->flipSide();
	}
	//proximity checks
	if ( std::abs(s1->getX() - s2->getX()) + std::abs(s1->getY() - s2->getY()) < s1->getFrameBox().w + 20 ) {
		fighter->setNextToOther(true);
	}
	else {
		if(fighter->getState() != "dragSpear")
			fighter->getSprite()->setCollisionOffset(7);
		fighter->setNextToOther(false);
	}
	
	if ( fighter->inInterruptibleState() || fighter->getCurrentlyRunningAnimator()->hasFinished() ) {		
		actionDispatcher[fighter->getNextAction()]();
	}
	if (fighter->getNextAction() == "forwardFlyKick" || fighter->getNextAction() == "backwardFlyKick" 
		|| fighter->getNextAction() == "forwardFlyPunch" || fighter->getNextAction() == "backwardFlyPunch"
		|| fighter->getNextAction() == "stationaryFlyPunch" || fighter->getNextAction() == "stationaryFlyKick") {
		actionDispatcher[fighter->getNextAction()]();
	}
}