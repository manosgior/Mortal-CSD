#pragma once

#include "../Engine/InputController.h"
#include "StateTransitions.h"
#include "../Engine/Sprite.h"
#include <string>
#include <map>
#include "../Engine/TickAnimation.h"
#include "../Engine/MovingAnimation.h"
#include "../Engine/FrameRangeAnimation.h"
#include "../Engine/FrameListAnimation.h"
#include "../Engine/FlashAnimation.h"
#include "../Engine/ScrollAnimation.h"
#include "../Engine/MovingPathAnimation.h"

#include "../Engine/TickAnimator.h"
#include "../Engine/MovingAnimator.h"
#include "../Engine/FrameRangeAnimator.h"
#include "../Engine/FrameListAnimator.h"
#include "../Engine/MovingPathAnimator.h"
#include "../Engine/FlashAnimator.h"
#include "../Engine/ScrollAnimator.h"
#include "../Engine/AnimatorHolder.h"
#include "Audio.h"
#include "FighterSprite.h"
#include "Projectile.h"

enum FighterStateType {
	HI_DEF = 0, LO_DEF, HI_NEU, LO_NEU, HI_ATK, LO_ATK, HI_HIT, LO_HIT, JMP
};

FighterStateType stringToStateTypeEnum(std::string s);

bool stateIsAtk(FighterStateType st);
bool stateIsNeu(FighterStateType st);
bool stateIsHit(FighterStateType st);
bool stateIsDef(FighterStateType st);
bool stateIsJmp(FighterStateType st);

class Fighter {
private:
	std::string																	name;
	FighterSprite																*sprite;

	InputController																inputController;
	StateTransitions															stateTransitions;
	std::string																	prevState;

	std::string																	nextAction;

	std::map<animid_t, std::tuple<MovingAnimation*,
		MovingAnimation*, const AnimationFilm*>>								allMovingAnims;
	std::map<animid_t, std::tuple<MovingPathAnimation*,
		MovingPathAnimation*, const AnimationFilm*>>							allMovingPathAnims;
	std::map<animid_t, std::tuple<FrameRangeAnimation*,
		FrameRangeAnimation*, const AnimationFilm*>>							allFrameRangeAnims;
	std::map<animid_t, std::tuple<FrameListAnimation*,
		FrameListAnimation *, const AnimationFilm*>>							allFrameListAnims;
	std::map<animid_t, std::pair<FlashAnimation*, const AnimationFilm*>>		allFlashAnims;
	std::map<animid_t, std::string>												animationType;

	MovingAnimator																movingAnimator;
	FrameRangeAnimator															frameRangeAnimator;
	FrameListAnimator															frameListAnimator;
	FlashAnimator																flashAnimator;
	MovingPathAnimator															movingPathAnimator;
	Animator																	*currentlyRunningAnimator;

	TickAnimator																tickAnimator;
	TickAnimation																*tickAnimation;
	
	int																			HP;
	int																			lastHP;

	bool																		isOnTheLeft;
	bool																		nextToOther;
	bool																		ignoreInput;
	bool																		flag;
	int																			wins;
	int																			losses;
	std::map<std::string, FighterStateType>										stateTypes;
	std::map<std::string, std::string>											strikeToHitState;
	std::map<std::string, std::pair<std::set<FighterStateType>, int>>			strikeInfo;
	

	int																			initProjectileOffsetLeft;
	int																			initProjectileOffsetRight;
	Dim																			initProjectileOffsetY;
	int																			projectileDmg;
	std::string																	projectileName;

	int																			initProjectile2OffsetLeft;
	int																			initProjectile2OffsetRight;
	Dim																			initProjectile2OffsetY;	
	int																			projectile2Dmg;
	std::string																	projectile2Name;

	bool																		strikeSuccess(std::string strike, Fighter *other);

public:
	Fighter(std::string name, FighterSprite *spr, bool onTheLeft);

	std::string				getName();
	FighterSprite*			getSprite();
	void					resetY();

	void					processReceivedInput(bool useMaximalEdges);
	InputController&		getInputController();
	StateTransitions&		getStateTransitions();

	void					setNextAction(std::string);
	std::string				getNextAction();

	void					addMovingAnimation(std::string name, MovingAnimation* anim, MovingAnimation* animInv, const AnimationFilm* film);
	void					addFrameRangeAnimation(std::string name, FrameRangeAnimation* anim, FrameRangeAnimation* animInv, const AnimationFilm* film);
	void					addFrameListAnimation(std::string name, FrameListAnimation* anim, FrameListAnimation* animInv, const AnimationFilm* film);
	void					addMovingPathAnimation(std::string name, MovingPathAnimation* anim, MovingPathAnimation* animInv, const AnimationFilm* film);
	void					addFlashAnimation(std::string name, FlashAnimation*, const AnimationFilm* film);

	void					setAnimationFilm(std::string);

	template <typename Tfunc>
	void					setOnFinish(const Tfunc &f) {
		currentlyRunningAnimator->setOnFinish(f);
	}
	
	Animator*				getCurrentlyRunningAnimator();
	const AnimationFilm*	getMovingPathAnimationFilm(std::string);

	void					startPlayingAnimation(std::string animationName, timestamp_t currTime);	

	void					setTickAnimation(TickAnimation*);
	void					startTickAnimator(timestamp_t);

	int						getHP();
	int						getLastHP();
	void					setHP(int hp);
	void					setHPforcefully(int hp);
	void                    setLastHP(int hp);

	void					flipSide();
	bool					isOnLeftSide();
	void					setNextToOther(bool proximity);
	bool					isNextToOther();

	bool					inInterruptibleState();
	std::string				getState();
	std::string				getPrevState();
	void					setState(std::string);
	void					addStateType(std::string state, FighterStateType type);
	FighterStateType		getStateType(std::string);
	void					mapStrikeToHitState(std::string strikeName, std::string hitStateName);
	void					setStrikeDamage(std::string strike, int dmg);	
	void					addStrikeCapability(std::string strike, FighterStateType type);

	void					attack(Fighter *other);
	void					setIgnoreInput(bool);
	void					applyDamage(int dmg);	
	int						getWins();
	void					incWins();
	int						getLosses();
	void					incLosses();
	bool					getFlag();
	void					setFlag(bool);

	Projectile				createProjectile(std::string projectile,timestamp_t currTime);
	Projectile				createProjectile2(std::string projectile, timestamp_t currTime);
	void					setProjectileStats(std::string name, int dmg, Dim leftOffset, Dim rightOffset, Dim yOffset);
	void					setProjectile2Stats(std::string name, int dmg, Dim leftOffset, Dim rightOffset, Dim yOffset);
};