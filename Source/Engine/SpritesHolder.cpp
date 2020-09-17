#include "SpritesHolder.h"

SpritesHolder SpritesHolder::holder = SpritesHolder();
SpritesHolder::SpritesByType SpritesHolder::sprites = SpritesByType();
std::map<int, SpritesHolder::SpriteList> SpritesHolder::zOrderHashtable = std::map<int, SpriteList>();

void SpritesHolder::Add(Sprite *s) {
	zOrderHashtable[s->getZOrder()].push_back(s);
	sprites[s->getType()].push_back(s);
}

void SpritesHolder::Remove(Sprite *s) {
	zOrderHashtable[s->getZOrder()].remove(s);
	sprites[s->getType()].remove(s);
}

void SpritesHolder::GetSprites(std::string type, SpriteList* result) const{
	auto i = sprites.find(type);
	if (i != sprites.end()) *result = i->second;
}

const SpritesHolder& SpritesHolder::Get() {
	return holder;
}

void SpritesHolder::displayAll(const ViewWindow *view) {
	for (auto layerSprites : zOrderHashtable) {
		for (auto currentSprite : layerSprites.second) {
			currentSprite->display(view);
		}
	}
}

void SpritesHolder::displayRunning(const ViewWindow *view, std::string arenaName) {
	for (auto layerSprites : zOrderHashtable) {
		for (auto currentSprite : layerSprites.second) {
			if (currentSprite->getType() != "menu" && currentSprite->getType() != "pretitle" && currentSprite->getType() != "titlescreen" && currentSprite->getType() != "select" && currentSprite->getType() != "arena") {
				currentSprite->display(view);
			}
		}
	}
}

void SpritesHolder::displayType(const ViewWindow *view, std::string type) {
	for (auto layerSprites : zOrderHashtable) {
		for (auto currentSprite : layerSprites.second) {
			if (currentSprite->getType() == type)currentSprite->display(view);
		}
	}
}