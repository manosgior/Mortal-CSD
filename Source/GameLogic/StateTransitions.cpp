#include "StateTransitions.h"
#include <assert.h>
#include <algorithm>
#include <vector>
#include <iostream>
const std::string StateTransitions::makeKey(const Input& input) const {
	std::string result;
	for (auto& s : input)
		result += "." + s;
	return result;
}
#include <iostream>
void StateTransitions::performDefaultTransition() {
	auto j(transitionTable.find(currentState + ".*"));
	if (j != transitionTable.end()) {
		j->second(); //perform the transition
	}
}

void StateTransitions::fireTransitions(const std::set<std::string>& keys) {
	for (auto& key : keys) {		
		auto i(transitionTable.find(currentState + makeKey({ key })));
		assert(i != transitionTable.end());
		i->second();//perform the transition
	}
}

void StateTransitions::insertByRetainingMaximalEdges(std::set<std::string>& keys, const std::string& fired) {
	std::vector<std::string> keysToDelete;

	for (auto& key : keys) {
		if (key.find(fired) != std::string::npos) //substring of existing key, ignore fired
			return;
		if (fired.find(key) != std::string::npos){	//superstring of existing key, ignore key
			keysToDelete.push_back(key);
		}
	}
	for (auto& key : keysToDelete) {
		keys.erase(key);
	}
	keys.insert(fired);
}

StateTransitions& StateTransitions::setState(const std::string& newState){
	currentState = newState; 
	return *this;
}

StateTransitions& StateTransitions::performTransitions(const Input& input, bool useMaximalEdges) {
	if (!input.empty()) {
		auto i(inputs.find(currentState)); //find all registered inputs for current state
		assert(i != inputs.end());
		auto& edges(i->second); //get the list of inputs

		std::set<std::string> fired; //set of edges that will be fired

		for (auto& edge : edges)
			//if this edge is a subset of the currently given input
			if (std::includes(input.begin(), input.end(), edge.begin(), edge.end())) {
				//find the pair of <transition function, next state> for this transition
				auto j(transitionTable.find(currentState + makeKey(edge)));
				assert(j != transitionTable.end());
				auto& key(j->first); //j is the concatenation of state plus current input (e.g. "S1.a.b")
				//save all fired transitions, based on whether or not only the maximal edge should execute
				for (auto& e : edge) {
					if (useMaximalEdges)
						insertByRetainingMaximalEdges(fired, e);
					else
						fired.insert(e);
				}
			}

		if (fired.empty())
			performDefaultTransition();
		else
			fireTransitions(fired);
	}
	else
		performDefaultTransition();
	return *this;
}

const std::string&	StateTransitions::getState() const { 
	return currentState; 
}

