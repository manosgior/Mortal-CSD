#pragma once
#include "../Engine/InputController.h"
#include "StateTransitions.h"
#include "../Engine/GTypes.h"
#include "../Engine/TickAnimation.h"
#include "../Engine/TickAnimator.h"
#include "../Engine/Animator.h"
#include "Menu.h"

class GeneralInputManager {
	InputController	inputController;
	StateTransitions stateTransitions;
	std::string state;
	std::vector<std::string> selections;
	std::string fighterSelections[2];
	std::string arenaSelection;
	Menu *currentMenu;
	timestamp_t t_offset;
	bool selectionMade;
	Dim selectionId;
public:
	GeneralInputManager(int);
	~GeneralInputManager();
	void resetGIM();
	void processReceivedInput(bool useMaximalEdges);
	InputController& getInputController();
	StateTransitions& getStateTransitions();
	void setState(std::string);
	std::string getState();
	void addSelection(std::string);
	std::string getSelection(int);
	void setSelection(int i, std::string);
	std::string getFighterSelection(int);
	void setFighterSelection(int i, std::string);
	void addArenaSelection(std::string);
	std::string getArenaSelection();
	void resetSelections();
	void setCurrentMenu(Menu *);
	Menu* getCurrentMenu();
	void setOffset(timestamp_t);
	bool getSelectionMade();
	void setSelectionMade(bool);
	Dim getSelectionId();
	void setSelectionId(Dim i);
	timestamp_t getOffset();
};

void createGIMInputController(GeneralInputManager *gim, std::string filepath1, std::string filepath2);
void createGIMTransitions(GeneralInputManager *gim, timestamp_t *currTime);
void onPaused(GeneralInputManager *gim, timestamp_t *currTime);
void onRunning(GeneralInputManager *gim, timestamp_t *currTime);
void setNextState(GeneralInputManager *gim, std::string);
void onMoveSelection(GeneralInputManager *gim, int i, Dim p);
void onConfirmSelection(GeneralInputManager *gim, Dim p);
void onExitGame();
void onFighterSelect(GeneralInputManager *gim, std::string selectedOption);
void onArenaSelect(GeneralInputManager *gim, std::string selectedOption);