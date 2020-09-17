#pragma once
#include "../Engine/InputController.h"
#include <set>
#include <functional>

class StateTransitions {
public:
	using Input = std::set<std::string>;
private:
	using StateToInput = std::map<std::string, std::list<Input>>;
	using TransitionTable = std::map<std::string, std::function<void(void)>>;
	TransitionTable			transitionTable;
	StateToInput			inputs;
	std::string				currentState;

	const std::string	makeKey(const Input& input) const;
	void				performDefaultTransition();
	void				fireTransitions(const std::set<std::string>& keys);
	void				insertByRetainingMaximalEdges(std::set<std::string>& keys, const std::string& fired);
public:
	//definition needs to be here because of template
	template <typename Tfunc>
	StateTransitions&	setTransition(const std::string& from, Input& input, const Tfunc& f) {
		if (!input.empty()) {
			transitionTable[from + makeKey(input)] = f;
			inputs[from].push_back(input);
		}
		else
			transitionTable[from + ".*"] = f;
		return *this;
	}
	StateTransitions&	setState(const std::string& newState);
	StateTransitions&	performTransitions(const Input& input, bool useMaximalEdges);
	const std::string&	getState() const;
	StateTransitions(StateTransitions&&) = delete;
	StateTransitions(const StateTransitions&) = delete;
	StateTransitions() = default;
};