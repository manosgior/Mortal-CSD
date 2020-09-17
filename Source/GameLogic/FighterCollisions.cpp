#include "FighterCollisions.h"
#include "../Engine/AnimationsHolder.h"
#include "../Engine/CollisionChecker.h"
#include "../Engine/AnimationFilmHolder.h"
#include "../Engine/AnimatorHolder.h"
#include <iostream>


FighterCollisionHandler::FighterCollisionHandler(){}

void cleanBlood(Sprite *blood) {
	blood->setVisibility(false);
}

std::string FighterCollisionHandler::selectBloodAnimation(int dmgReceived) {
	if (dmgReceived > 25)
		return "bloodFlyHeavy";
	else if (dmgReceived > 15)
		return "bloodFlyMedium";
	else if (dmgReceived > 10)
		return "bloodFlyLight";
	return "";
}

void FighterCollisionHandler::playHitSound(int oldHp1, int oldHp2) {
	auto hurtFighter = (f1->getHP() < oldHp1) ? (f1) : (f2);
	auto dmgReceived = (hurtFighter == f1) ? (oldHp1 - hurtFighter->getHP()) : (oldHp2 - hurtFighter->getHP());
	if (dmgReceived > 25) {
		if(hurtFighter->getState() != "hitThrow")
			AudioHolder::playSound("meatRoundhouseKick");
	}
	else if (dmgReceived > 15) {
		if (rand() % 2)
			AudioHolder::playSound("meatKick");
		else
			AudioHolder::playSound("meatKneeKick");
	}
	else if (dmgReceived > 10) {
		if (rand() % 2)
			AudioHolder::playSound("meatPunch");
		else
			AudioHolder::playSound("meatPunch2");
	}
}


void FighterCollisionHandler::sprayBlood(int oldHp1, int oldHp2) {
	auto hurtFighter = (f1->getHP() < oldHp1) ? (f1) : (f2);
	auto dmgReceived = (hurtFighter == f1) ? (oldHp1 - hurtFighter->getHP()) : (oldHp2 - hurtFighter->getHP());
	if (hurtFighter->isOnLeftSide()) {
		bloodSprite1->setX(hurtFighter->getSprite()->getX() + 5);
		bloodSprite2->setX(hurtFighter->getSprite()->getX() + 5);
	}
	else {
		bloodSprite1->setX(hurtFighter->getSprite()->getX() + hurtFighter->getSprite()->getFrameBox().w - 5);
		bloodSprite2->setX(hurtFighter->getSprite()->getX() + hurtFighter->getSprite()->getFrameBox().w - 5);
	}
	bloodSprite1->setY(f1->getSprite()->getY() + 5);
	bloodSprite2->setY(f1->getSprite()->getY() + 15);
	auto anim = selectBloodAnimation(dmgReceived);
	if (anim != "") {
		if (!hurtFighter->isOnLeftSide()) {
			anim = anim + "_inverted";
		}
		bloodAnimator1->start(bloodSprite1, AnimationsHolder::getMovingPathAnim(anim), *currTime);
		//bring out the big guns
		if (anim == "bloodFlyHeavy" || anim == "bloodFlyHeavy_inverted") {
			bloodAnimator2->start(bloodSprite2, AnimationsHolder::getMovingPathAnim(anim), *currTime);
			bloodAnimator2->setOnFinish(std::bind(cleanBlood, bloodSprite2));
			AnimatorHolder::markAsRunning(bloodAnimator2);
			bloodSprite2->setVisibility(true);
		}
		bloodAnimator1->setOnFinish(std::bind(cleanBlood, bloodSprite1));
		AnimatorHolder::markAsRunning(bloodAnimator1);
		bloodSprite1->setVisibility(true);
	}
}

void FighterCollisionHandler::operator()(Sprite *caller, Sprite *arg) {
	if (arg->getType() != "fighter") {
		return;
	}

	FighterStateType st1 = f1->getStateType(f1->getState()), st2 = f2->getStateType(f2->getState());
	int hp1 = f1->getHP(), hp2 = f2->getHP();
	if (stateIsAtk(st1) && stateIsAtk(st2)) {
		if (rand() % 2 == 0) {
			f1->attack(f2);
		}
		else {
			f2->attack(f1);
		}
	}
	else if (stateIsAtk(st1))
		f1->attack(f2);

	else if (stateIsAtk(st2))
		f2->attack(f1);
	sprayBlood(hp1, hp2);
	playHitSound(hp1, hp2);
}

FighterCollisionHandler* FighterCollisionHandler::clone() const {
	auto tmp = new FighterCollisionHandler();
	tmp->f1 = f1;
	tmp->f2 = f2;
	tmp->currTime = currTime;
	tmp->bloodSprite1 = bloodSprite1;
	tmp->bloodAnimator1 = bloodAnimator1;
	tmp->bloodSprite2 = bloodSprite2;
	tmp->bloodAnimator2 = bloodAnimator2;
	return tmp;
}


extern void returnToIdle(Fighter *fighter, timestamp_t *currTime);
extern void onSpearRelease(Fighter *fighter, timestamp_t *currTime);



void resetJumpAnimation(Fighter *fighter, timestamp_t *currTime, int pathIndex, timestamp_t resetDelay, MovingPathAnimation *anim) {
	auto p = anim->getPath();
	p[pathIndex].delay = resetDelay;
	anim->setPath(p);
	if(fighter->getState() == "frozenOnAir")
		returnToIdle(fighter, currTime);
}

void IceCollisionHandler::operator()(Sprite *caller, Sprite *arg) {
	if (arg != p.spr) {
		return;
	}

	this->p.animator->stop(false);
	p.spr->setFrame(7);
	p.spr->setX((f->isOnLeftSide()) ? (caller->getX() + 40) : (caller->getX() - 40));
	p.spr->move(0, 20);
	p.spr->setHAlign((f->isOnLeftSide()) ? (HFrameAlign::LEFT) : (HFrameAlign::RIGHT));
	p.spr->setVAlign(VFrameAlign::CENTER);
	this->p.animator->start(p.spr, AnimationsHolder::getMovingPathAnim("iceCollision"), *currTime);
	this->p.animator->setOnFinish<std::function<void()>>(std::bind(&destroyProjectile, arg, caller));
	AudioHolder::playSound("iceHit");
	if (f->getStateType(f->getState()) == JMP && !f->getCurrentlyRunningAnimator()->hasFinished()) {
		auto index = ((MovingPathAnimator *)f->getCurrentlyRunningAnimator())->getPathIndex();
		auto runningAnimation = ((MovingPathAnimator *)f->getCurrentlyRunningAnimator())->getAnimation();
		auto p = runningAnimation->getPath();
		auto oldDelay = p[index].delay;
		p[index].delay = AnimationsHolder::getMovingPathAnim("frozen")->getPath()[0].delay;
		((MovingPathAnimator *)f->getCurrentlyRunningAnimator())->getAnimation()->setPath(p);

		f->setOnFinish(std::bind(resetJumpAnimation, f, currTime, index, oldDelay, runningAnimation));
		f->setState("frozenOnAir");
		f->setNextAction("frozenOnAir");
	}
	else {
		auto tmp = f->getSprite()->getFilm();
		auto currFrame = f->getSprite()->getFrame();
		f->startPlayingAnimation("frozen", *currTime);
		f->getSprite()->setAnimationFilm(tmp);
		((MovingPathAnimator *)f->getCurrentlyRunningAnimator())->getAnimation()->calibratePathEntryFrames(0, { currFrame });
		f->setOnFinish(std::bind(returnToIdle, f, currTime));
		f->setState("frozen");
		f->setNextAction("frozen");
	}
}

void onDragFinish(Sprite *pspr, Fighter * wielder, Fighter *f, timestamp_t *currTime) {
	returnToIdle(wielder, currTime);
	wielder->getSprite()->setCollisionOffset(0);
	destroyProjectile(pspr, f->getSprite());
}

void afterDragFinish(Fighter *f2, timestamp_t *currTime) {
	returnToIdle(f2, currTime);
}

void AfterSpearHandler::operator()(Sprite *caller, Sprite *arg) {/*
	if (arg != f2->getSprite()) return;
	f2->getCurrentlyRunningAnimator()->stop(false);
	f2->setState("dizzy");
	f2->setNextAction("dizzy");

	f1->getSprite()->markCollisionHandlerForRemoval(this);*/
}

AfterSpearHandler* AfterSpearHandler::clone() const {
	auto tmp = new AfterSpearHandler();
	tmp->f1 = f1;
	tmp->f2 = f2;
	tmp->currTime = currTime;
	return tmp;
}

void onSpearCollision(Fighter *wielder, Fighter *other, Projectile p, int startFrame, Sprite * caller, timestamp_t *currTime) {
	wielder->setState("dragSpear");
	wielder->setNextAction("dragSpear");
	wielder->startPlayingAnimation("scorpion_dragSpear", *currTime);
	other->setState("impaledMove");
	other->setNextAction("impaledMove");
	other->startPlayingAnimation("impaledMove", *currTime);
	if(rand()%2)
		AudioHolder::playSound("comeHere");
	else
		AudioHolder::playSound("getOverHere");
	auto col = AfterSpearHandler();
	col.f1 = wielder;
	col.f2 = other;
	col.currTime = currTime;
	wielder->getSprite()->addCollisionHandler(col);
	wielder->getSprite()->setCollisionOffset(32);
	p.animator->start(p.spr, AnimationsHolder::getMovingPathAnim(wielder->isOnLeftSide() ? "spearDrag" : "spearDrag_inverted"), *currTime);
	p.animator->setPathIndex(startFrame);
	p.animator->setOnFinish<std::function<void()>>(std::bind(&onDragFinish, p.spr, wielder, other, currTime));
}

int calibrateSpearFrame(int d, int frameDiff, int totalFrames) {
	int tmp = totalFrames * frameDiff;
	int i;
	for (i = 1; i <= totalFrames; i++) {
		if (std::abs(d - (i * frameDiff + 44)) <= tmp) {
			tmp = d - i * frameDiff;
		}
		else {
			return totalFrames - i;
		}
	}
	return 0;
}

void SpearCollisionHandler::operator()(Sprite *caller, Sprite *arg) {
	if (arg != p.spr) {
		return;
	}
	this->p.animator->stop(false);
	int offset = wielder->isOnLeftSide() ? 50 : 50;	
	int p1 = f->getSprite()->getX();
	if (!(f->isOnLeftSide())) p1 += f->getSprite()->getFrameBox().w;
	int p2 = wielder->getSprite()->getX();
	if (wielder->isOnLeftSide()) p2 += wielder->getSprite()->getFrameBox().w;
	int diff = p1 - p2;
	this->p.spr->setAnimationFilm(AnimationFilmHolder::Get().GetFilm("scorpion_spearProjectileStabbed"));
	int startFrame = calibrateSpearFrame(std::abs(p1 - p2), 5, 45);
	std::cout << startFrame << std::endl;
	this->p.animator->start(p.spr, AnimationsHolder::getMovingPathAnim(f->isOnLeftSide() ? "spearCollision_inverted" : "spearCollision"), *currTime);
	std::vector<int> nf;
	nf.push_back(startFrame);
	nf.push_back(startFrame);
	this->p.animator->getAnimation()->calibratePathEntryFrames(0, nf);
	this->p.spr->setX(wielder->isOnLeftSide() ? wielder->getSprite()->getX() + wielder->getSprite()->getFrameBox().w - offset : wielder->getSprite()->getX() - p.spr->getFrameBox().w + offset);
	this->p.animator->setOnFinish<std::function<void()>>(std::bind(&onSpearCollision, wielder, f, p, startFrame, caller, currTime));
	
	f->applyDamage(p.dmg);
	f->resetY();
	f->setState("impaled");
	f->setNextAction("impaled");
	f->startPlayingAnimation("impaled", *currTime);
}

IceCollisionHandler* IceCollisionHandler::clone() const {
	auto tmp = new IceCollisionHandler();
	tmp->f = f;
	tmp->p = p;
	tmp->currTime = currTime;
	return tmp;
}

SpearCollisionHandler* SpearCollisionHandler::clone() const {
	auto tmp = new SpearCollisionHandler();
	tmp->f = f;
	tmp->wielder = wielder;
	tmp->p = p;
	tmp->currTime = currTime;
	return tmp;
}

void destroyProjectile(Sprite *p, Sprite *f) {
	p->setVisibility(false);
	CollisionChecker::Get().cancelPair(p, f);
}