#pragma once
#include "Fighter.h"
#include <map>
#include <string>
#include <functional>


struct FighterCollisionHandler : public CollisionHandler {
	Fighter *f1, *f2;
	timestamp_t *currTime;
	MovingPathAnimator *bloodAnimator1;
	MovingPathAnimator *bloodAnimator2;
	Sprite *bloodSprite1;
	Sprite *bloodSprite2;
	FighterCollisionHandler();
	void sprayBlood(int oldHp1, int oldHp2);
	void playHitSound(int oldHp1, int oldHp2);
	std::string selectBloodAnimation(int dmgReceived);
	virtual void operator()(Sprite *caller, Sprite *arg) override;
	virtual FighterCollisionHandler *clone() const;
};

struct IceCollisionHandler : public CollisionHandler {
	Fighter *f;
	Projectile p;
	timestamp_t *currTime;
	virtual void operator()(Sprite *caller, Sprite *arg) override;
	virtual IceCollisionHandler *clone() const;
};

struct SpearCollisionHandler : public CollisionHandler {
	Fighter *f;
	Fighter *wielder;
	Projectile p;
	timestamp_t *currTime;
	virtual void operator()(Sprite *caller, Sprite *arg) override;
	virtual SpearCollisionHandler *clone() const;
};

struct AfterSpearHandler : public CollisionHandler {
	Fighter *f1, *f2;
	timestamp_t *currTime;
	virtual void operator()(Sprite *caller, Sprite *arg) override;
	virtual AfterSpearHandler *clone() const;
};

void destroyProjectile(Sprite *p, Sprite *f);