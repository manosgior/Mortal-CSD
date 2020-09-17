#pragma once
#include "../Engine/Sprite.h"

class Menu {
	std::vector<int> rows;
	std::vector<Sprite> Sprites;
	Dim width, height;
	Point selection1, selection2;
	Point selectionSpriteOffset;
	bool singleSelect;
	unsigned int firstButton;
	unsigned int confirmedSelections;
	std::vector<std::string> options;
	Layer bl = { 400, 254, 0, 0, 0, 0 };
public:
	Menu(std::string path_sprite, std::string path_config, bool _singleSelect, std::string id);
	void addSpritesToHolder();
	Dim getWidth();
	Dim getHeight();
	Dim getSelectionIndex(int);
	void setRow(int, Dim);
	Dim getRow(int);
	Dim getSelection(int);
	void setSelection(int, Dim);
	void updateSelectionSprite(int);
	void addConfirmedSelection(int p);
	unsigned int getConfirmedSelections();
	void resetConfirmedSelection();
	std::string getSelectionOption(int);
};