#pragma once

#include "Fighter.h"

struct FighterInfoFilePaths {
	std::string controlsFilepath; 
	std::string animationInfoFilePath; 
	std::string invertedPostfix; 
	std::string basicInfoFilePath;
	std::string strikeInfoFilePath;
	std::string specialStrikeInfoFilePath;
};

void constructFighterTransitions(Fighter *f);

void constructFighterInputController(Fighter *f, std::string filepath);

void constructSubzero(Fighter *f, struct FighterInfoFilePaths infoPaths);

void constructScorpion(Fighter *f, struct FighterInfoFilePaths infoPaths);