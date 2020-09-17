#pragma once

#include <list>
#include <map>
#include <allegro5/allegro.h>
#include "Sprite.h"
#include "ViewWindow.h"

class SpritesHolder {
public:
	typedef std::list<Sprite *> SpriteList;
	typedef std::map<std::string, SpriteList> SpritesByType;
private:
	static SpritesHolder holder;
protected:
	static SpritesByType sprites;
	static std::map<int, SpriteList> zOrderHashtable;
public:
	void Add(Sprite *s);
	void Remove(Sprite *s);
	void GetSprites(std::string type, SpriteList *result) const;	
	void displayAll(const ViewWindow *view);
	void displayRunning(const ViewWindow *view, std::string);
	void displayType(const ViewWindow *view, std::string type);
	static const SpritesHolder& Get();
};