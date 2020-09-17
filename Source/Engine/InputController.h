#pragma once
#include <allegro5/allegro.h>
#include <map>
#include <string>
#include <list>
#include <set>

class InputController {
	using key_combination = std::list<std::pair<std::string, char>>;
	using Actions = std::list<std::pair<key_combination, std::string>>;

	std::map<std::string, int> keyMappings;
	Actions registeredActions;
	std::set<std::string> runningLogicalActions;

	static std::list<InputController*> allControllers;

	bool testKey(const std::string& keyName, const char timesPressed[], char keyPresses);
	bool testKeys(const key_combination& keys, const char timesPressed[]);
	void setLogical(std::string id);


public:
	InputController();
	~InputController();

	void addKeyBinding(std::string keyName, int keyCode);
	void reverseForward();
	void handle(const char isPressed[]);
	void addAction(const key_combination& keys, const std::string& logical);
	static void handleAll(const char timesPressed[]);
	const std::set<std::string>& getLogical(void) const;
	int getKeycode(std::string keyName);
};