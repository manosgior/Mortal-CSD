#include "ConstructFighter.h"
#include "../Utility/json-develop/include/nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include "../Engine/AnimationFilmHolder.h"
#include "../Engine/AnimationsHolder.h"

using json = nlohmann::json;

void constructFighterTransitions(Fighter *f) {
	StateTransitions &st = f->getStateTransitions();
	auto lowPunch = StateTransitions::Input{ "lowPunch" };
	auto highKick = StateTransitions::Input{ "highKick" };
	auto lowKick = StateTransitions::Input{ "lowKick" };
	auto backHighKick = StateTransitions::Input{"back.highKick"};
	auto sweepKick = StateTransitions::Input{ "back.lowKick" };
	auto forwardBlock = StateTransitions::Input{"forward.block"};
	auto block = StateTransitions::Input{ "block" };
	auto down = StateTransitions::Input{ "down" };
	auto downBlock = StateTransitions::Input{ "down.block" };
	auto highPunch = StateTransitions::Input{ "highPunch" };
	auto forwardUp = StateTransitions::Input{ "forward.up" };
	auto forward = StateTransitions::Input{ "forward" };
	auto backUp = StateTransitions::Input{ "back.up" };
	auto back = StateTransitions::Input{ "back" };
	auto up = StateTransitions::Input{ "up" };


	//st.setTransition<std::function<void()>>("idle", lowPunch, std::bind(&Fighter::setNextAction, f, "lowPunch"));

	st.setTransition<std::function<void()>>("idle", forward, std::bind(&Fighter::setNextAction, f, "moveForward"));
	st.setTransition<std::function<void()>>("idle", back, std::bind(&Fighter::setNextAction, f, "moveBackwards"));
	st.setTransition<std::function<void()>>("moveForward", forward, std::bind(&Fighter::setNextAction, f, "moveForward"));
	st.setTransition<std::function<void()>>("moveForward", lowKick, std::bind(&Fighter::setNextAction, f, "lowKick"));
	st.setTransition<std::function<void()>>("moveForward", highKick, std::bind(&Fighter::setNextAction, f, "highKick"));
	st.setTransition<std::function<void()>>("moveForward", lowPunch, std::bind(&Fighter::setNextAction, f, "lowPunch1"));
	st.setTransition<std::function<void()>>("moveForward", highPunch, std::bind(&Fighter::setNextAction, f, "highPunch1"));
	st.setTransition<std::function<void()>>("moveForward", forwardBlock, std::bind(&Fighter::setNextAction, f, "highBlock"));
	st.setTransition<std::function<void()>>("moveForward", up, std::bind(&Fighter::setNextAction, f, "rightJump"));
	st.setTransition<std::function<void()>>("moveBackwards", up, std::bind(&Fighter::setNextAction, f, "leftJump"));
	st.setTransition<std::function<void()>>("moveBackwards", back, std::bind(&Fighter::setNextAction, f, "moveBackwards"));
	st.setTransition<std::function<void()>>("moveBackwards", block, std::bind(&Fighter::setNextAction, f, "highBlock"));
	st.setTransition<std::function<void()>>("moveBackwards", lowPunch, std::bind(&Fighter::setNextAction, f, "lowPunch1"));
	st.setTransition<std::function<void()>>("moveBackwards", highPunch, std::bind(&Fighter::setNextAction, f, "highPunch1"));
	st.setTransition<std::function<void()>>("moveBackwards", lowKick, std::bind(&Fighter::setNextAction, f, "sweepKick"));
	st.setTransition<std::function<void()>>("moveBackwards", highKick, std::bind(&Fighter::setNextAction, f, "roundhouseKick"));
	st.setTransition<std::function<void()>>("idle", down, std::bind(&Fighter::setNextAction, f, "duck"));
	st.setTransition<std::function<void()>>("duck", down, std::bind(&Fighter::setNextAction, f, "duck"));

	st.setTransition<std::function<void()>>("idle", highKick, std::bind(&Fighter::setNextAction, f, "highKick"));
	st.setTransition<std::function<void()>>("idle", lowKick, std::bind(&Fighter::setNextAction, f, "lowKick"));
	st.setTransition<std::function<void()>>("idle", backHighKick, std::bind(&Fighter::setNextAction, f, "roundhouseKick"));
	st.setTransition<std::function<void()>>("idle", sweepKick, std::bind(&Fighter::setNextAction, f, "sweepKick"));
	st.setTransition<std::function<void()>>("duck", highKick, std::bind(&Fighter::setNextAction, f, "lowToHighKick"));
	st.setTransition<std::function<void()>>("duck", lowKick, std::bind(&Fighter::setNextAction, f, "lowToHighKick"));

	st.setTransition<std::function<void()>>("idle", highPunch, std::bind(&Fighter::setNextAction, f, "highPunch1"));
	st.setTransition<std::function<void()>>("idle", lowPunch, std::bind(&Fighter::setNextAction, f, "lowPunch1"));
	st.setTransition<std::function<void()>>("highPunch1", highPunch, std::bind(&Fighter::setNextAction, f, "highPunch2"));
	st.setTransition<std::function<void()>>("highPunch2", highPunch, std::bind(&Fighter::setNextAction, f, "highPunch1"));
	st.setTransition<std::function<void()>>("lowPunch1", lowPunch, std::bind(&Fighter::setNextAction, f, "lowPunch2"));
	st.setTransition<std::function<void()>>("lowPunch2", lowPunch, std::bind(&Fighter::setNextAction, f, "lowPunch1"));
	st.setTransition<std::function<void()>>("duck", highPunch, std::bind(&Fighter::setNextAction, f, "uppercut"));
	st.setTransition<std::function<void()>>("duck", lowPunch, std::bind(&Fighter::setNextAction, f, "uppercut"));

	st.setTransition<std::function<void()>>("idle", block, std::bind(&Fighter::setNextAction, f, "highBlock"));
	st.setTransition<std::function<void()>>("highBlock", block, std::bind(&Fighter::setNextAction, f, "highBlock"));
	st.setTransition<std::function<void()>>("duck", downBlock, std::bind(&Fighter::setNextAction, f, "lowBlock"));
	st.setTransition<std::function<void()>>("lowBlock", downBlock, std::bind(&Fighter::setNextAction, f, "lowBlock"));

	st.setTransition<std::function<void()>>("idle", forwardUp, std::bind(&Fighter::setNextAction, f, "rightJump"));
	st.setTransition<std::function<void()>>("idle", backUp, std::bind(&Fighter::setNextAction, f, "leftJump"));
	st.setTransition<std::function<void()>>("idle", up, std::bind(&Fighter::setNextAction, f, "jump"));

	st.setTransition<std::function<void()>>("rightJump", highKick, std::bind(&Fighter::setNextAction, f, "forwardFlyKick"));
	st.setTransition<std::function<void()>>("rightJump", lowKick, std::bind(&Fighter::setNextAction, f, "forwardFlyKick"));	

	st.setTransition<std::function<void()>>("leftJump", highKick, std::bind(&Fighter::setNextAction, f, "backwardFlyKick"));
	st.setTransition<std::function<void()>>("leftJump", lowKick, std::bind(&Fighter::setNextAction, f, "backwardFlyKick"));

	st.setTransition<std::function<void()>>("rightJump", lowPunch, std::bind(&Fighter::setNextAction, f, "forwardFlyPunch"));
	st.setTransition<std::function<void()>>("rightJump", highPunch, std::bind(&Fighter::setNextAction, f, "forwardFlyPunch"));
	
	st.setTransition<std::function<void()>>("leftJump", lowPunch, std::bind(&Fighter::setNextAction, f, "backwardFlyPunch"));
	st.setTransition<std::function<void()>>("leftJump", highPunch, std::bind(&Fighter::setNextAction, f, "backwardFlyPunch"));

	st.setTransition<std::function<void()>>("jump", lowPunch, std::bind(&Fighter::setNextAction, f, "stationaryFlyPunch"));
	st.setTransition<std::function<void()>>("jump", highPunch, std::bind(&Fighter::setNextAction, f, "stationaryFlyPunch"));
	
	st.setTransition<std::function<void()>>("jump", lowKick, std::bind(&Fighter::setNextAction, f, "stationaryFlyKick"));
	st.setTransition<std::function<void()>>("jump", highKick, std::bind(&Fighter::setNextAction, f, "stationaryFlyKick"));


	auto anyInput = StateTransitions::Input{ "*" };
	st.setTransition<std::function<void()>>("idle", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));

	st.setTransition<std::function<void()>>("highKick", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("lowKick", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("roundhouseKick", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("lowToHighKick", anyInput, std::bind(&Fighter::setNextAction, f, "duck"));
	st.setTransition<std::function<void()>>("sweepKick", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("kneeKick", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));


	st.setTransition<std::function<void()>>("moveForward", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("moveBackwards", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("duck", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));

	st.setTransition<std::function<void()>>("highPunch1", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("highPunch2", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("lowPunch1", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("lowPunch2", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("uppercut", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("elbowStrike", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("throw", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));

	st.setTransition<std::function<void()>>("highBlock", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("lowBlock", anyInput, std::bind(&Fighter::setNextAction, f, "duck"));

	st.setTransition<std::function<void()>>("rightJump", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("leftJump", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("jump", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));

	st.setTransition<std::function<void()>>("forwardFlyKick", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("backwardFlyKick", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));

	st.setTransition<std::function<void()>>("forwardFlyPunch", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("backwardFlyPunch", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("stationaryFlyPunch", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
	st.setTransition<std::function<void()>>("stationaryFlyKick", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));

	st.setTransition<std::function<void()>>("flipToLeft", anyInput, std::bind(&Fighter::setNextAction, f, "flipToLeft"));
	st.setTransition<std::function<void()>>("flipToRight", anyInput, std::bind(&Fighter::setNextAction, f, "flipToRight"));

	st.setTransition<std::function<void()>>("hitHigh", anyInput, std::bind(&Fighter::setNextAction, f, "hitHigh"));
	st.setTransition<std::function<void()>>("stagger", anyInput, std::bind(&Fighter::setNextAction, f, "stagger"));
	st.setTransition<std::function<void()>>("hitLow", anyInput, std::bind(&Fighter::setNextAction, f, "hitLow"));
	st.setTransition<std::function<void()>>("hitLowWhenHigh", anyInput, std::bind(&Fighter::setNextAction, f, "hitLowWhenHigh"));
	st.setTransition<std::function<void()>>("hitLowWhenHighStatic", anyInput, std::bind(&Fighter::setNextAction, f, "hitLowWhenHighStatic"));
	st.setTransition<std::function<void()>>("falling", anyInput, std::bind(&Fighter::setNextAction, f, "falling"));	
	st.setTransition<std::function<void()>>("dead", anyInput, std::bind(&Fighter::setNextAction, f, "dead"));
	st.setTransition<std::function<void()>>("burned", anyInput, std::bind(&Fighter::setNextAction, f, "burned"));
	st.setTransition<std::function<void()>>("deadBurned", anyInput, std::bind(&Fighter::setNextAction, f, "deadBurned"));
	st.setTransition<std::function<void()>>("decapitated", anyInput, std::bind(&Fighter::setNextAction, f, "decapitated"));
	st.setTransition<std::function<void()>>("deadDecapitated", anyInput, std::bind(&Fighter::setNextAction, f, "deadDecapitated"));
	st.setTransition<std::function<void()>>("dizzy", anyInput, std::bind(&Fighter::setNextAction, f, "dizzy"));
	st.setTransition<std::function<void()>>("deadTripped", anyInput, std::bind(&Fighter::setNextAction, f, "deadTripped"));
	st.setTransition<std::function<void()>>("falling_big", anyInput, std::bind(&Fighter::setNextAction, f, "falling_big"));
	st.setTransition<std::function<void()>>("hitThrow", anyInput, std::bind(&Fighter::setNextAction, f, "hitThrow"));
	st.setTransition<std::function<void()>>("tripped", anyInput, std::bind(&Fighter::setNextAction, f, "tripped"));
	st.setTransition<std::function<void()>>("hitHighBlock", anyInput, std::bind(&Fighter::setNextAction, f, "hitHighBlock"));
	st.setTransition<std::function<void()>>("hitLowBlock", anyInput, std::bind(&Fighter::setNextAction, f, "hitLowBlock"));
	st.setTransition<std::function<void()>>("frozen", anyInput, std::bind(&Fighter::setNextAction, f, "frozen"));
	st.setTransition<std::function<void()>>("frozenOnAir", anyInput, std::bind(&Fighter::setNextAction, f, "frozenOnAir"));
	st.setTransition<std::function<void()>>("impaled", anyInput, std::bind(&Fighter::setNextAction, f, "impaled"));
	st.setTransition<std::function<void()>>("impaledMove", anyInput, std::bind(&Fighter::setNextAction, f, "impaledMove"));
	st.setTransition<std::function<void()>>("gettingUp", anyInput, std::bind(&Fighter::setNextAction, f, "gettingUp"));
	st.setTransition<std::function<void()>>("victory", anyInput, std::bind(&Fighter::setNextAction, f, "victory"));

	st.setState("idle");
	f->setNextAction("idle");
}

void constructFighterInputController(Fighter *f, std::string filepath) {
	InputController &ic = f->getInputController();

	std::ifstream inputFile;
	inputFile.open(filepath, std::ifstream::in);
	assert(!(inputFile.fail()));
	json inputs;

	while (!inputFile.eof()) {
		inputFile >> inputs;
		inputFile.peek();
		ic.addKeyBinding("forward", inputs["forward"].get<int>());
		ic.addKeyBinding("back", inputs["back"].get<int>());
		ic.addKeyBinding("up", inputs["up"].get<int>());
		ic.addKeyBinding("down", inputs["down"].get<int>());

		ic.addKeyBinding("highPunch", inputs["highPunch"].get<int>());
		ic.addKeyBinding("lowPunch", inputs["lowPunch"].get<int>());
		ic.addKeyBinding("highKick", inputs["highKick"].get<int>());
		ic.addKeyBinding("lowKick", inputs["lowKick"].get<int>());
		ic.addKeyBinding("block", inputs["block"].get<int>());
	}
	
	ic.addAction({ { "forward", 1 } }, "forward");
	ic.addAction({ { "back", 1 } }, "back");
	ic.addAction({ { "up", 1 } }, "up");
	ic.addAction({ { "down", 1 } }, "down");
	ic.addAction({ { "highPunch", 1 } }, "highPunch");
	ic.addAction({ { "lowPunch", 1 } }, "lowPunch");
	ic.addAction({ { "highKick", 1 } }, "highKick");
	ic.addAction({ { "lowKick", 1 } }, "lowKick");	
	ic.addAction({ { "block", 1 } }, "block");
	ic.addAction({ { "forward", 1}, {"block", 1} }, "forward.block");

	ic.addAction({ { "back", 1 }, { "highKick", 1} }, "back.highKick");
	ic.addAction({ { "back", 1 }, { "lowKick", 1 } }, "back.lowKick");
	ic.addAction({ { "down", 1 }, { "block", 1 } }, "down.block");
	ic.addAction({ { "forward", 1 }, { "up", 1 } }, "forward.up");
	ic.addAction({ { "back", 1 }, { "up", 1 } }, "back.up");	
}

void constructFighterAnimations(Fighter *f, std::string animationInfoFilePath, std::string invertedPostfix) {
	std::ifstream inputFile;
	inputFile.open(animationInfoFilePath, std::ifstream::in);
	assert(!(inputFile.fail()));
	json animationInput;

	while (!inputFile.eof()) {
		inputFile >> animationInput;
		inputFile.peek();
		std::string animType = animationInput["type"].get<std::string>();
		std::string animName = animationInput["name"].get<std::string>();
		std::string animationId = animationInput["animationId"].get<std::string>();
		std::string animationFilmId = animationInput["animationFilmId"].get<std::string>();

		if (animType == "moving") {
			f->addMovingAnimation(animName, AnimationsHolder::getMovingAnim(animationId),
				AnimationsHolder::getMovingAnim(animationId + invertedPostfix), AnimationFilmHolder::Get().GetFilm(animationFilmId));
		}
		else if (animType == "frameList") {
			f->addFrameListAnimation(animName, AnimationsHolder::getFrameListAnim(animationId),
				AnimationsHolder::getFrameListAnim(animationId + invertedPostfix), AnimationFilmHolder::Get().GetFilm(animationFilmId));
		}
		else if (animType == "frameRange") {
			f->addFrameRangeAnimation(animName, AnimationsHolder::getFrameRangeAnim(animationId),
				AnimationsHolder::getFrameRangeAnim(animationId + invertedPostfix), AnimationFilmHolder::Get().GetFilm(animationFilmId));
		}
		else if (animType == "movingPath") {
			f->addMovingPathAnimation(animName, AnimationsHolder::getMovingPathAnim(animationId),
				AnimationsHolder::getMovingPathAnim(animationId + invertedPostfix), AnimationFilmHolder::Get().GetFilm(animationFilmId));
		}
		else if (animType == "flash")
			f->addFlashAnimation(animName, AnimationsHolder::getFlashAnim(animationId),
				AnimationFilmHolder::Get().GetFilm(animationFilmId));
		else
			assert(false);

	}
}

void constructFighterBasicInfo(Fighter *f, std::string basicInfoFilePath) {
	std::ifstream inputFile;
	inputFile.open(basicInfoFilePath, std::ifstream::in);
	assert(!(inputFile.fail()));
	json striketohitInput;
	inputFile >> striketohitInput;
	f->setProjectileStats(striketohitInput["projectileName"].get < std::string>(),
		striketohitInput["damage"].get<int>(), striketohitInput["leftOffset"].get<Dim>(), striketohitInput["rightOffset"].get<Dim>(),
		striketohitInput["yOffset"].get<Dim>());
	inputFile >> striketohitInput;
	f->setProjectile2Stats(striketohitInput["projectile2Name"].get < std::string>(), striketohitInput["damage"].get<int>(), striketohitInput["leftOffset"].get<Dim>(),
		striketohitInput["rightOffset"].get<Dim>(),	striketohitInput["yOffset"].get<Dim>());
	inputFile >> striketohitInput;
	for (json::iterator it = striketohitInput.begin(); it != striketohitInput.end(); ++it) {
		for (auto stateEntry : it.value())
			f->addStateType(stateEntry.get<std::string>(), stringToStateTypeEnum(it.key()));
	}
}

void constructFighterStrikes(Fighter *f, std::string striketohitInfoFilePath) {
	std::ifstream inputFile;
	inputFile.open(striketohitInfoFilePath, std::ifstream::in);
	assert(!(inputFile.fail()));
	json striketohitInput;

	while (!inputFile.eof()) {
		inputFile >> striketohitInput;
		inputFile.peek();

		std::string strike = striketohitInput["strike"].get<std::string>();
		int dmg = striketohitInput["damage"].get<int>();
		f->setStrikeDamage(strike, dmg);

		for (auto stateToHitState : striketohitInput["state"]) {
			json::iterator it = stateToHitState.begin();
			f->mapStrikeToHitState(strike + "." + it.key(), it.value());
		}

		for (auto hittableState : striketohitInput["hittableStates"]) {
			f->addStrikeCapability(strike, stringToStateTypeEnum(hittableState.get<std::string>()));
		}
	}
}

void constructSubzero(Fighter *f, struct FighterInfoFilePaths infoPaths) {
	constructFighterInputController(f, infoPaths.controlsFilepath);
	constructFighterTransitions(f);
	constructFighterAnimations(f, infoPaths.animationInfoFilePath, infoPaths.invertedPostfix);
	constructFighterBasicInfo(f, infoPaths.basicInfoFilePath);
	constructFighterStrikes(f, infoPaths.strikeInfoFilePath);
	constructFighterStrikes(f, infoPaths.specialStrikeInfoFilePath);

	InputController &ic = f->getInputController();
	StateTransitions &st = f->getStateTransitions();

	ic.addAction({ { "down", 1 },{ "forward", 1 }, { "lowPunch", 1 } }, "down.forward.lowPunch");
	ic.addAction({ { "back", 1 },{ "lowKick", 1 },{ "highKick", 1 } }, "back.lowKick.highKick");
	ic.addAction({ { "back", 1 },{ "down", 1 },{ "highPunch", 1 } }, "back.down.highPunch");

	auto downForwardLowPunch = StateTransitions::Input{ "down.forward.lowPunch" };
	auto backLowKickHighKick = StateTransitions::Input{ "back.lowKick.highKick" };
	auto backDownHighPunch = StateTransitions::Input{ "back.down.highPunch" };
	auto anyInput = StateTransitions::Input{ "*" };

	st.setTransition<std::function<void()>>("idle", downForwardLowPunch, std::bind(&Fighter::setNextAction, f, "throwIce"));
	st.setTransition<std::function<void()>>("throwIce", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));

	st.setTransition<std::function<void()>>("idle", backLowKickHighKick, std::bind(&Fighter::setNextAction, f, "slide"));

	st.setTransition<std::function<void()>>("idle", backDownHighPunch, std::bind(&Fighter::setNextAction, f, "subzeroFatality"));

	st.setTransition<std::function<void()>>("slide", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));

	st.setTransition<std::function<void()>>("subzeroFatality", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));


}

void constructScorpion(Fighter *f, struct FighterInfoFilePaths infoPaths) {
	constructFighterInputController(f, infoPaths.controlsFilepath);
	constructFighterTransitions(f);
	constructFighterAnimations(f, infoPaths.animationInfoFilePath, infoPaths.invertedPostfix);
	constructFighterBasicInfo(f, infoPaths.basicInfoFilePath);
	constructFighterStrikes(f, infoPaths.strikeInfoFilePath);
	constructFighterStrikes(f, infoPaths.specialStrikeInfoFilePath);

	InputController &ic = f->getInputController();
	StateTransitions &st = f->getStateTransitions();

	ic.addAction({ { "back", 1 }, { "lowPunch", 1 } }, "back.lowPunch");
	ic.addAction({ { "down", 1 }, { "back", 1 }, { "lowPunch", 1 } }, "down.back.lowPunch");
	ic.addAction({ { "up", 1 },  {"down", 1}, { "block", 1 } }, "up.down.block");

	auto backLowPunch = StateTransitions::Input{ "back.lowPunch" };	
	auto downBackLowPunch = StateTransitions::Input{ "down.back.lowPunch" };
	auto upDownBlock = StateTransitions::Input{ "up.down.block" };

	st.setTransition<std::function<void()>>("moveBackwards", backLowPunch, std::bind(&Fighter::setNextAction, f, "throwSpear"));

	st.setTransition<std::function<void()>>("idle", downBackLowPunch, std::bind(&Fighter::setNextAction, f, "teleportPunchPrep"));

	st.setTransition<std::function<void()>>("idle", upDownBlock, std::bind(&Fighter::setNextAction, f, "scorpionFatality"));

	auto anyInput = StateTransitions::Input{ "*" };

	st.setTransition<std::function<void()>>("throwSpear", anyInput, std::bind(&Fighter::setNextAction, f, "dragSpear"));

	st.setTransition<std::function<void()>>("dragSpear", anyInput, std::bind(&Fighter::setNextAction, f, "dragSpear"));

	st.setTransition<std::function<void()>>("teleportPunchPrep", anyInput, std::bind(&Fighter::setNextAction, f, "teleportPunch"));

	st.setTransition<std::function<void()>>("teleportPunch", anyInput, std::bind(&Fighter::setNextAction, f, "teleportPunchAttack"));

	st.setTransition<std::function<void()>>("teleportPunchAttack", anyInput, std::bind(&Fighter::setNextAction, f, "teleportPunchAttack"));

	st.setTransition<std::function<void()>>("scorpionFatality", anyInput, std::bind(&Fighter::setNextAction, f, "idle"));
}