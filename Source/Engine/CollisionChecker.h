#pragma once
#include <utility>
#include <set>
#include <map>
#include <algorithm>
#include "Sprite.h"

class CollisionChecker {
private:
	typedef std::pair<Sprite*, Sprite*> Pair;
	std::set<Pair> pairs;
	std::map<Pair, bool> wasInCollision;

public:
	void registerPair(Sprite* s1, Sprite* s2, bool notifyOncePerCollision);
	void cancelPair(Sprite* s1, Sprite* s2);
	void check(void);
	static CollisionChecker& Get();

private:
	CollisionChecker();
	static CollisionChecker collch;
};