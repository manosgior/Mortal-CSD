#pragma once
#include "Fighter.h"
#include <map>
#include <string>
#include <functional>

class FighterTickAnimations {
public:
	using onActionFunction = std::function<void(void)>;
private:
	Fighter *fighter, *otherFighter;
	timestamp_t *currTime;
	ViewWindow *viewWindow;
	std::map<std::string, onActionFunction> actionDispatcher;
	void basicSetup();

public:
	FighterTickAnimations(Fighter *fighter, Fighter *otherFighter, timestamp_t *currTime, ViewWindow *viewWindow) { 
		this->fighter = fighter;
		this->otherFighter = otherFighter;
		this->currTime = currTime;
		this->viewWindow = viewWindow;
	}
	void setUpSubzeroTickAnimation();
	void setUpScorpionTickAnimation();
	void onTickFunction();
};