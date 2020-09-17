#include "InputController.h"

#include <iostream>

std::list<InputController*> InputController::allControllers = std::list<InputController*>();


InputController::InputController(void) {
	allControllers.push_back(this);
}

InputController::~InputController() {
	allControllers.remove(this);
}

bool InputController::testKey(const std::string& keyName, const char timesPressed[], char keyPresses) {
	return timesPressed[keyMappings[keyName]] >= keyPresses;
}

bool InputController::testKeys(const key_combination& keys, const char timesPressed[]) {
	for (auto& key : keys)
		if (!testKey(key.first, timesPressed, key.second))
			return false;
	return true;
}

void InputController::setLogical(std::string id) {	
	runningLogicalActions.insert(id);
}

void InputController::addKeyBinding(std::string keyName, int keyCode) {
	keyMappings[keyName] = keyCode;
}

void InputController::handle(const char timesPressed[]) {
	runningLogicalActions.clear();
	for (auto& i : registeredActions)
		if (testKeys(i.first, timesPressed)) {
			setLogical(i.second);
		}
}

void InputController::addAction(const key_combination& keys, const std::string& logical){
	registeredActions.push_back(std::make_pair(keys, logical));
}

void InputController::handleAll(const char timesPressed[]) {
	for (auto* handler : allControllers)
		handler->handle(timesPressed);
}

const std::set<std::string>& InputController::getLogical(void) const {
	return runningLogicalActions;
}

void InputController::reverseForward() {
	int tmp = keyMappings["forward"];
	keyMappings["forward"] = keyMappings["back"];
	keyMappings["back"] = tmp;
}

int InputController::getKeycode(std::string keyName) {
	return keyMappings[keyName];
}
