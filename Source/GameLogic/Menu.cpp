#include "Menu.h"
#include "../Engine/AnimationFilmHolder.h"
#include "../Engine/SpritesHolder.h"
#include "../Utility/configReaders.h"
#include "../Utility/json-develop/include/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

Menu::Menu(std::string path_sprite, std::string path_config, bool _singleSelect, std::string id) : selection1({ 0,0 }), selection2({ 0,0 }), singleSelect(_singleSelect), confirmedSelections(0) {
	AnimationFilmHolder& holder = AnimationFilmHolder::Get();
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
		firstButton = config["FirstButtonIndex"].get<unsigned int>();
		selectionSpriteOffset.x = config["SelectionSpriteOffsetX"].get<Dim>();
		selectionSpriteOffset.y = config["SelectionSpriteOffsetY"].get<Dim>();
		for (auto row : config["Rows"]) {
			rows.push_back(row.get<int>());
		}
		for (auto sprite : config["Sprites"]) {
			Sprite s = Sprite(sprite[0].get<int>(), sprite[1].get<int>(), sprite[2].get<int>(), &bl, 
				holder.GetFilm(sprite[3].get<std::string>()), HFrameAlign::LEFT, VFrameAlign::TOP, id, "nostate");
			if (s.getZOrder() == 1) s.setVisibility(false);
			Sprites.push_back(s);
		}
		for (auto option : config["Options"]) {
			options.push_back(option.get<std::string>());
		}
	}
}

void Menu::addSpritesToHolder() {
	SpritesHolder h = SpritesHolder::Get();
	for (unsigned int i = 0; i < Sprites.size(); i++) {
		h.Add(&Sprites[i]);
	}
}

Dim Menu::getWidth() {
	return width;
}

Dim Menu::getHeight() {
	return height;
}

Dim Menu::getSelectionIndex(int p) {
	Dim i = 0;
	Dim row = p == 1 ? selection1.x : selection2.x;
	Dim selection = p == 1 ? selection1.y : selection2.y;
	for (Dim j = 0; j < row; j++) {
		i += rows[j];
	}
	i += selection;
	return i;
}

void Menu::setRow(int p, Dim i) {
	int offset;
	if (i < rows.size()) {
		if (p == 1) {
			selection1.x = i;
			if (selection1.y >= rows[i]) selection1.y = rows[i] - 1;
			//if (options[getSelectionIndex(p)] == "illegal") selection1.y++;
			if (singleSelect) {
				selection2.x = i;
				if (selection2.y >= rows[i]) selection2.y = rows[i] - 1;
				//if (options[getSelectionIndex(p)] == "illegal") selection2.y++;
			}
		}
		else {
			selection2.x = i;
			if (selection2.y >= rows[i]) selection2.y = rows[i] - 1;
			if (singleSelect) {
				selection1.x = i;
				if (selection1.y >= rows[i]) selection1.y = rows[i] - 1;
			}
		}
	}
}

void Menu::setSelection(int p, Dim i) {
	Dim row = p == 1 ? selection1.x : selection2.x;
	if (i < rows[row]) {
		if (p == 1) {
			selection1.y = i;
			if (singleSelect) selection2.y = i;
		}
		else {
			selection2.y = i;
			if (singleSelect) selection1.y = i;
		}
	}
}

Dim Menu::getSelection(int p) {
	return p == 1 ? selection1.y : selection2.y;
}

Dim Menu::getRow(int p) {
	return p == 1 ? selection1.x : selection2.x;
}

void Menu::updateSelectionSprite(int p) {
	Dim selind;
	if (singleSelect) selind = Sprites.size() - 1;
	else if (p == 1) selind = Sprites.size() - 2; 
	else selind = Sprites.size() - 1;
	Sprites[selind].setX(Sprites[firstButton + getSelectionIndex(p)].getX() - selectionSpriteOffset.x);
	Sprites[selind].setY(Sprites[firstButton + getSelectionIndex(p)].getY() - selectionSpriteOffset.y);
}

void Menu::addConfirmedSelection(int p) {
	confirmedSelections++;
	Dim selind;
	if (singleSelect) return;
	else if (p == 1) selind = Sprites.size() - 2;
	else selind = Sprites.size() - 1;
	Sprites[selind - 2].setX(Sprites[selind].getX());
	Sprites[selind - 2].setY(Sprites[selind].getY());
	Sprites[selind - 2].setVisibility(true);
}

void Menu::resetConfirmedSelection() {
	confirmedSelections = 0;
	if (singleSelect) return;
	Sprites[Sprites.size() - 3].setVisibility(false);
	Sprites[Sprites.size() - 4].setVisibility(false);
}

unsigned int Menu::getConfirmedSelections() {
	return confirmedSelections;
}

std::string Menu::getSelectionOption(int p) {
	return options[getSelectionIndex(p)];
}