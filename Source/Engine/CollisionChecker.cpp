#include "CollisionChecker.h"

CollisionChecker CollisionChecker::collch = CollisionChecker();

CollisionChecker& CollisionChecker::Get() {
	return collch;
}

void CollisionChecker::registerPair(Sprite *s1, Sprite *s2, bool notifyOncePerCollision) {
	if (CollisionChecker::pairs.find(Pair(s2, s1)) == CollisionChecker::pairs.end()) {
		auto x = CollisionChecker::pairs.insert(CollisionChecker::Pair(s1, s2));
		//pair didn't exist already
		if (x.second && notifyOncePerCollision) {
			wasInCollision[Pair(s1, s2)] = false;
		}
	}
	
}

void CollisionChecker::cancelPair(Sprite *s1, Sprite *s2) {
	CollisionChecker::pairs.erase(Pair(s1, s2));
	CollisionChecker::pairs.erase(Pair(s2, s1));
}

void CollisionChecker::check(void) {
	for (auto pair : pairs) {
		if (wasInCollision.find(pair) == wasInCollision.end())
			pair.first->collisionCheck(pair.second, true);
		else if (wasInCollision[pair] == false)
			wasInCollision[pair] = pair.first->collisionCheck(pair.second, true);
		else
			wasInCollision[pair] = pair.first->collisionCheck(pair.second, false);
	}
}


CollisionChecker::CollisionChecker(){}