#include "../Engine/Sprite.h"
#include "../Engine/MovingPathAnimator.h"
#include <functional>

struct Projectile {
	Sprite	*spr;
	MovingPathAnimator *animator;
	int dmg;
};