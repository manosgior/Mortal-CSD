#include "GIM.h"
#include "../Utility/json-develop/include/nlohmann/json.hpp"
#include <fstream>
#include "../Engine/AnimatorHolder.h"
#include "Audio.h"
#include <iostream>
using json = nlohmann::json; 

void createGIMInputController(GeneralInputManager *gim, std::string filepath1, std::string filepath2) {
	std::ifstream inputFile;
	InputController& ic = gim->getInputController();
	inputFile.open(filepath1, std::ifstream::in);
	assert(!(inputFile.fail()));

	json inputs;

	while (!inputFile.eof()) {
		inputFile >> inputs;
		inputFile.peek();
		ic.addKeyBinding("up1", inputs["up"].get<int>());
		ic.addKeyBinding("down1", inputs["down"].get<int>());
		ic.addKeyBinding("left1", inputs["back"].get<int>());
		ic.addKeyBinding("right1", inputs["forward"].get<int>());
		ic.addKeyBinding("start1", inputs["start"].get<int>());
		ic.addKeyBinding("select1", inputs["lowPunch"].get<int>());
	}

	inputFile.close();
	inputFile.open(filepath2, std::ifstream::in);
	assert(!(inputFile.fail()));

	json inputs2;

	while (!inputFile.eof()) {
		inputFile >> inputs2;
		inputFile.peek();
		ic.addKeyBinding("up2", inputs2["up"].get<int>());
		ic.addKeyBinding("down2", inputs2["down"].get<int>());
		ic.addKeyBinding("left2", inputs2["forward"].get<int>());
		ic.addKeyBinding("right2", inputs2["back"].get<int>());
		ic.addKeyBinding("start2", inputs2["start"].get<int>());
		ic.addKeyBinding("select2", inputs2["lowPunch"].get<int>());
	}

	ic.addAction({ {"up1", 1} }, "move_selection1_up");
	ic.addAction({ {"down1", 1} }, "move_selection1_down");
	ic.addAction({ {"left1", 1} }, "move_selection1_left");
	ic.addAction({ {"right1", 1} }, "move_selection1_right");
	ic.addAction({ {"start1", 1} }, "action_start1");
	ic.addAction({ {"select1", 1} }, "action_select1");
	ic.addAction({ {"up2", 1} }, "move_selection2_up");
	ic.addAction({ {"down2", 1} }, "move_selection2_down");
	ic.addAction({ {"left2", 1} }, "move_selection2_left");
	ic.addAction({ {"right2", 1} }, "move_selection2_right");
	ic.addAction({ {"start2", 1} }, "action_start2");
	ic.addAction({ {"select2", 1} }, "action_select2");
}

void createGIMTransitions(GeneralInputManager *gim, timestamp_t *currTime) {
	StateTransitions& st = gim->getStateTransitions();
	auto up1 = StateTransitions::Input{ "move_selection1_up" };
	auto down1 = StateTransitions::Input{ "move_selection1_down" };
	auto left1 = StateTransitions::Input{ "move_selection1_left" };
	auto right1 = StateTransitions::Input{ "move_selection1_right" };
	auto start1 = StateTransitions::Input{ "action_start1" };
	auto select1 = StateTransitions::Input{ "action_select1" };
	auto up2 = StateTransitions::Input{ "move_selection2_up" };
	auto down2 = StateTransitions::Input{ "move_selection2_down" };
	auto left2 = StateTransitions::Input{ "move_selection2_left" };
	auto right2 = StateTransitions::Input{ "move_selection2_right" };
	auto start2 = StateTransitions::Input{ "action_start2" };
	auto select2 = StateTransitions::Input{ "action_select2" };
	auto any = StateTransitions::Input{ "*" };
	st.setTransition<std::function<void()>>("running", start1, std::bind(&onPaused, gim, currTime));
	st.setTransition<std::function<void()>>("running", start2, std::bind(&onPaused, gim, currTime));
	st.setTransition<std::function<void()>>("paused", start1, std::bind(&onRunning, gim, currTime));
	st.setTransition<std::function<void()>>("paused", start2, std::bind(&onRunning, gim, currTime));
	st.setTransition<std::function<void()>>("paused", up1, std::bind(&onMoveSelection, gim, 1, 1));
	st.setTransition<std::function<void()>>("paused", up2, std::bind(&onMoveSelection, gim, 1, 2));
	st.setTransition<std::function<void()>>("paused", down1, std::bind(&onMoveSelection, gim, 3, 1));
	st.setTransition<std::function<void()>>("paused", down2, std::bind(&onMoveSelection, gim, 3, 2));
	st.setTransition<std::function<void()>>("paused", left1, std::bind(&onMoveSelection, gim, 0, 1));
	st.setTransition<std::function<void()>>("paused", left2, std::bind(&onMoveSelection, gim, 0, 2));
	st.setTransition<std::function<void()>>("paused", right1, std::bind(&onMoveSelection, gim, 2, 1));
	st.setTransition<std::function<void()>>("paused", right2, std::bind(&onMoveSelection, gim, 2, 2));
	st.setTransition<std::function<void()>>("paused", select1, std::bind(&onConfirmSelection, gim, 1));
	st.setTransition<std::function<void()>>("paused", select2, std::bind(&onConfirmSelection, gim, 2));
	st.setTransition<std::function<void()>>("paused", any, std::bind(&setNextState, gim, "paused"));
	st.setTransition<std::function<void()>>("running", any, std::bind(&setNextState, gim, "running"));
	st.setTransition<std::function<void()>>("ExitGame", any, std::bind(&onExitGame));
	st.setTransition<std::function<void()>>("Screenshot", start1, std::bind(&setNextState, gim, "paused"));
	st.setTransition<std::function<void()>>("Screenshot", start2, std::bind(&setNextState, gim, "paused"));
	st.setTransition<std::function<void()>>("Screenshot", select1, std::bind(&setNextState, gim, "paused"));
	st.setTransition<std::function<void()>>("Screenshot", select2, std::bind(&setNextState, gim, "paused"));
	st.setTransition<std::function<void()>>("pretitle", start1, std::bind(&setNextState, gim, "titlescreen"));
	st.setTransition<std::function<void()>>("pretitle", start2, std::bind(&setNextState, gim, "titlescreen"));
	st.setTransition<std::function<void()>>("titlescreen", start1, std::bind(&setNextState, gim, "select00"));
	st.setTransition<std::function<void()>>("titlescreen", start2, std::bind(&setNextState, gim, "select00"));
	st.setTransition<std::function<void()>>("select00", start1, std::bind(&onConfirmSelection, gim, 1));
	st.setTransition<std::function<void()>>("select00", start2, std::bind(&onConfirmSelection, gim, 2));
	st.setTransition<std::function<void()>>("select00", up1, std::bind(&onMoveSelection, gim, 1, 1));
	st.setTransition<std::function<void()>>("select00", up2, std::bind(&onMoveSelection, gim, 1, 2));
	st.setTransition<std::function<void()>>("select00", down1, std::bind(&onMoveSelection, gim, 3, 1));
	st.setTransition<std::function<void()>>("select00", down2, std::bind(&onMoveSelection, gim, 3, 2));
	st.setTransition<std::function<void()>>("select00", left1, std::bind(&onMoveSelection, gim, 0, 1));
	st.setTransition<std::function<void()>>("select00", left2, std::bind(&onMoveSelection, gim, 0, 2));
	st.setTransition<std::function<void()>>("select00", right1, std::bind(&onMoveSelection, gim, 2, 1));
	st.setTransition<std::function<void()>>("select00", right2, std::bind(&onMoveSelection, gim, 2, 2));
	st.setTransition<std::function<void()>>("select10", start2, std::bind(&onConfirmSelection, gim, 2));
	st.setTransition<std::function<void()>>("select10", up2, std::bind(&onMoveSelection, gim, 1, 2));
	st.setTransition<std::function<void()>>("select10", down2, std::bind(&onMoveSelection, gim, 3, 2));
	st.setTransition<std::function<void()>>("select10", left2, std::bind(&onMoveSelection, gim, 0, 2));
	st.setTransition<std::function<void()>>("select10", right2, std::bind(&onMoveSelection, gim, 2, 2));
	st.setTransition<std::function<void()>>("select01", start1, std::bind(&onConfirmSelection, gim, 1));
	st.setTransition<std::function<void()>>("select01", up1, std::bind(&onMoveSelection, gim, 1, 1));
	st.setTransition<std::function<void()>>("select01", down1, std::bind(&onMoveSelection, gim, 3, 1));
	st.setTransition<std::function<void()>>("select01", left1, std::bind(&onMoveSelection, gim, 0, 1));
	st.setTransition<std::function<void()>>("select01", right1, std::bind(&onMoveSelection, gim, 2, 1));
	st.setTransition<std::function<void()>>("select11", any, std::bind(&setNextState, gim, "arena"));
	st.setTransition<std::function<void()>>("arena", start1, std::bind(&onConfirmSelection, gim, 1));
	st.setTransition<std::function<void()>>("arena", start2, std::bind(&onConfirmSelection, gim, 2));
	st.setTransition<std::function<void()>>("arena", up1, std::bind(&onMoveSelection, gim, 1, 1));
	st.setTransition<std::function<void()>>("arena", up2, std::bind(&onMoveSelection, gim, 1, 2));
	st.setTransition<std::function<void()>>("arena", down1, std::bind(&onMoveSelection, gim, 3, 1));
	st.setTransition<std::function<void()>>("arena", down2, std::bind(&onMoveSelection, gim, 3, 2));
	st.setTransition<std::function<void()>>("arena", left1, std::bind(&onMoveSelection, gim, 0, 1));
	st.setTransition<std::function<void()>>("arena", left2, std::bind(&onMoveSelection, gim, 0, 2));
	st.setTransition<std::function<void()>>("arena", right1, std::bind(&onMoveSelection, gim, 2, 1));
	st.setTransition<std::function<void()>>("arena", right2, std::bind(&onMoveSelection, gim, 2, 2));
	st.setTransition<std::function<void()>>("arena", any, std::bind(&setNextState, gim, "arena"));
	st.setTransition<std::function<void()>>("subzero", any, std::bind(&onFighterSelect, gim, "subzero"));
	st.setTransition<std::function<void()>>("scorpion", any, std::bind(&onFighterSelect, gim, "scorpion"));
	st.setTransition<std::function<void()>>("courtyard", any, std::bind(&onArenaSelect, gim, "courtyard"));
	st.setTransition<std::function<void()>>("throneroom", any, std::bind(&onArenaSelect, gim, "throneroom"));
	st.setTransition<std::function<void()>>("ready", any, std::bind(&setNextState, gim, "ready"));
	gim->setState("pretitle");
}

void setNextState(GeneralInputManager *gim, std::string nextState) {
	gim->setState(nextState);
	gim->setSelectionMade(false);
	gim->resetSelections();
}

void onPaused(GeneralInputManager *gim, timestamp_t *currTime) {
	gim->getCurrentMenu()->resetConfirmedSelection();
	gim->setOffset(*currTime);
	gim->setState("paused");
}

void onRunning(GeneralInputManager *gim, timestamp_t *currTime) {
	gim->getCurrentMenu()->resetConfirmedSelection();
	gim->setOffset(*currTime - gim->getOffset());
	AnimatorHolder::timeshiftAll(gim->getOffset());
	gim->setState("running");
}

void onMoveSelection(GeneralInputManager *gim, int i, Dim p) {
	switch (i) {
	case 0:
		gim->getCurrentMenu()->setSelection(p, gim->getCurrentMenu()->getSelection(p) - 1);
		break;
	case 1:
		gim->getCurrentMenu()->setRow(p, gim->getCurrentMenu()->getRow(p) - 1);
		break;
	case 2:
		gim->getCurrentMenu()->setSelection(p, gim->getCurrentMenu()->getSelection(p) + 1);
		break;
	case 3:
		gim->getCurrentMenu()->setRow(p, gim->getCurrentMenu()->getRow(p) + 1);
		break;
	}
	gim->getCurrentMenu()->updateSelectionSprite(p);
}

bool selectionValid(GeneralInputManager *gim, std::string selection) {
	if (gim->getState().find("select") != std::string::npos) {
		if (selection != "subzero" && selection != "scorpion") return false;
	}
	if (gim->getState() == "arena") {
		if (selection != "courtyard" && selection != "throneroom") return false;
	}
	if (gim->getState() == "paused") {
		if (selection != "ExitGame" && selection != "Screenshot") return false;
	}
	return true;
}

void onConfirmSelection(GeneralInputManager *gim, Dim p) {
	if (!gim->getSelectionMade()) {
		std::string selectedOption = gim->getCurrentMenu()->getSelectionOption(p);
		if (selectionValid(gim, selectedOption)) {
			gim->getCurrentMenu()->addConfirmedSelection(p);
			gim->setState(selectedOption);
			gim->setSelectionId(p);
			gim->setSelectionMade(true);
		}
	}
}

void onExitGame() {
	exit(0);
}

void onFighterSelect(GeneralInputManager *gim, std::string selectedOption) {
	AudioHolder::playSound(selectedOption+"Select");
	if (gim->getFighterSelection(0) == "noselect" && gim->getFighterSelection(1) == "noselect" && gim->getSelectionId() == 1) {
		gim->setState("select10");
		gim->setFighterSelection(0, selectedOption);
	}
	else if (gim->getFighterSelection(0) == "noselect" && gim->getFighterSelection(1) == "noselect" && gim->getSelectionId() == 2) {
		gim->setState("select01");
		gim->setFighterSelection(1, selectedOption);
	}
	else if (gim->getFighterSelection(1) == "noselect" && gim->getFighterSelection(0) != "noselect") {
		gim->setState("select11");
		gim->setFighterSelection(1, selectedOption);
	}
	else if (gim->getFighterSelection(0) == "noselect" && gim->getFighterSelection(1) != "noselect") {
		gim->setState("select11");
		gim->setFighterSelection(0, selectedOption);
	}
	else {
		//assert(false);
	}
	gim->setSelectionMade(false);
}

void onArenaSelect(GeneralInputManager *gim, std::string selectedOption) {
	gim->setState("ready");
	gim->addArenaSelection(selectedOption);
}

GeneralInputManager::GeneralInputManager(int i) : inputController(), stateTransitions() {
	fighterSelections[0] = "noselect";
	fighterSelections[1] = "noselect";
	selectionMade = false;
}

void GeneralInputManager::resetGIM() {
	fighterSelections[0] = "noselect";
	fighterSelections[1] = "noselect";
	arenaSelection = "noselect";
	selectionMade = false;
	resetSelections();
	currentMenu->resetConfirmedSelection();
}

GeneralInputManager::~GeneralInputManager() {}

void GeneralInputManager::processReceivedInput(bool useMaximalEdges) {
	stateTransitions.performTransitions(inputController.getLogical(), useMaximalEdges);
}

InputController& GeneralInputManager::getInputController() {
	return inputController;
}

StateTransitions& GeneralInputManager::getStateTransitions() {
	return stateTransitions;
}

void GeneralInputManager::setState(std::string s) {
	state = s;
}

std::string GeneralInputManager::getState() {
	return state;
}

void GeneralInputManager::addSelection(std::string s) {
	selections.push_back(s);
}

std::string GeneralInputManager::getSelection(int i) {
	return selections[i];
}

void GeneralInputManager::setSelection(int i, std::string s) {
	selections[i] = s;
}

std::string GeneralInputManager::getFighterSelection(int i) {
	return fighterSelections[i];
}

void GeneralInputManager::setFighterSelection(int i, std::string s) {
	fighterSelections[i] = s;
}

void GeneralInputManager::addArenaSelection(std::string s) {
	arenaSelection = s;
}

std::string GeneralInputManager::getArenaSelection() {
	return arenaSelection;
}

void GeneralInputManager::resetSelections() {
	selections = std::vector<std::string>();
}

void GeneralInputManager::setCurrentMenu(Menu * m) {
	currentMenu = m;
}

Menu* GeneralInputManager::getCurrentMenu() {
	return currentMenu;
}

bool GeneralInputManager::getSelectionMade() {
	return selectionMade;
}

void GeneralInputManager::setSelectionMade(bool b) {
	selectionMade = b;
}

Dim GeneralInputManager::getSelectionId() {
	return selectionId;
}

void GeneralInputManager::setSelectionId(Dim i) {
	selectionId = i;
}

void GeneralInputManager::setOffset(timestamp_t t) {
	t_offset = t;
}

timestamp_t GeneralInputManager::getOffset() {
	return t_offset;
}