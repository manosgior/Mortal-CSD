#include "Arena.h"
#include "../Utility/json-develop/include/nlohmann/json.hpp"
#include <fstream>
#include "../Engine/AnimationFilmHolder.h"
#include "../Engine/SpritesHolder.h"
#include "../Utility/configReaders.h"
#include <iostream>

using json = nlohmann::json;

Arena::Arena(std::string path_sprite, std::string path_config, std::string _name) : name(_name), tickAnimator() {
	AnimationFilmHolder &holder = AnimationFilmHolder::Get();
	//tickAnimator = TickAnimator();
	tickAnimation = nullptr;
	readBitmapConfig(path_sprite);
	std::ifstream inputFile;
	inputFile.open(path_config, std::ifstream::in);
	assert(!(inputFile.fail()));
	json config;
	while (!inputFile.eof()) {
		inputFile >> config;
		inputFile.peek();
		width = config["Width"].get<Dim>();
		height = config["Height"].get<Dim>();
		int i = 0;
		for (auto fpos : config["FighterPos"]) {
			fighterPos[i++] = fpos;
		}
		i = 0;
		for (auto border : config["Borders"]) {
			borders[i++] = border;
		}
		i = 0;
		for (auto vBorder : config["ViewBorders"]) {
			viewBorders[i++] = vBorder;
		}
		for (auto layer : config["Layers"]) {
			Layer l = { layer[0], layer[1],0,0,0,0 };
			Layers.push_back(l);
		}
		for (auto sprite : config["Sprites"]) {
			Sprite s = Sprite(sprite[0].get<int>(), sprite[1].get<int>(), sprite[2].get<int>(), &Layers[sprite[2].get<int>()], holder.GetFilm(sprite[3].get<std::string>()), HFrameAlign::LEFT, VFrameAlign::TOP, "arenaRunning", name);
			Sprites.push_back(s);
		}	
	}
}

Arena::~Arena() {

}

void Arena::addSpritesToHolder() {
	SpritesHolder h = SpritesHolder::Get();
	for (unsigned int i = 0; i < Sprites.size(); i++) {
		h.Add(&Sprites[i]);
	}

}

void Arena::removeSpritesFromHolder() {
	SpritesHolder h = SpritesHolder::Get();
	for (unsigned int i = 0; i < Sprites.size(); i++) {
		h.Remove(&Sprites[i]);
	}
}

void Arena::reRegister() {
	AnimatorHolder::Register(&tickAnimator);
}

Dim Arena::getWidth() {
	return width;
}

Dim Arena::getHeight() {
	return height;
}

std::string Arena::getName() {
	return name;
}
void Arena::setName(std::string _name) {
	name = _name;
}

Dim Arena::getFighterPos(int i) {
	return fighterPos[i];
}

void Arena::setTickAnimation(TickAnimation* t) {
	tickAnimation = t;
}

void Arena::startTickAnimator(timestamp_t currTime) {
	tickAnimator.start(tickAnimation, currTime);
	AnimatorHolder::markAsRunning(&tickAnimator);
}

void Arena::advanceSpriteFrame(int i) {
	Sprites[i].setFrame((Sprites[i].getFrame() + 1) % Sprites[i].getFilm()->getTotalFrames());
}

void Arena::setSpriteFrame(int i, int j) {
	assert(j >= 0 && j < Sprites[i].getFilm()->getTotalFrames());
	Sprites[i].setFrame(j);
}

Dim Arena::getBorder(int i) {
	return borders[i];
}

Dim Arena::getViewBorder(int i) {
	return viewBorders[i];
}


