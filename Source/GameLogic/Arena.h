#pragma once
#include <vector>
#include <map>
#include "../Engine/Sprite.h"
#include "../Engine/TickAnimator.h"
#include "../Engine/TickAnimation.h"

class Arena {
	std::vector<Layer> Layers;
	std::vector<Sprite> Sprites;
	std::string name;
	Dim width;
	Dim height;
	Dim borders[4];
	Dim viewBorders[4];
	Dim fighterPos[4];
	TickAnimator tickAnimator;
	TickAnimation *tickAnimation;
public:
	Arena(std::string path_sprite, std::string path_config, std::string _name);
	~Arena();
	void addSpritesToHolder();
	void removeSpritesFromHolder();
	Dim getWidth();
	Dim getHeight();
	Dim getFighterPos(int);
	Dim getBorder(int);
	Dim getViewBorder(int);
	std::string getName();
	void setName(std::string);
	void advanceSpriteFrame(int);
	void setSpriteFrame(int, int);
	void setTickAnimation(TickAnimation*);
	void startTickAnimator(timestamp_t);
	void reRegister();

};