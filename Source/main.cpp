#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Engine/AnimationFilmHolder.h"
#include "Engine/Sprite.h"
#include "Engine/SpritesHolder.h"
#include <iostream>
#include <fstream>
#include <Windows.h>

#include "Utility/configReaders.h"
#include "Engine/CollisionChecker.h"
#include "Engine/FrameRangeAnimator.h"
#include "Engine/FrameListAnimator.h"
#include "Engine/FrameListAnimation.h"
#include "Engine/FlashAnimator.h"
#include "Engine/FlashAnimation.h"
#include "Engine/MovingAnimation.h"
#include "Engine/MovingAnimator.h"
#include "Engine/MovingPathAnimation.h"
#include "Engine/MovingPathAnimator.h"
#include "Engine/AnimatorHolder.h"
#include "Engine/ViewWindow.h"
#include "Engine/ScrollAnimation.h"
#include "Engine/ScrollAnimator.h"
#include "Engine/InputController.h"
#include "GameLogic/StateTransitions.h"
#include "GameLogic/Fighter.h"
#include "Engine/TickAnimation.h"
#include "GameLogic/ConstructFighter.h"
#include "GameLogic/ViewWinTickAnimations.h"

#include "GameLogic/GIM.h"
#include "GameLogic/Menu.h"
#include "GameLogic/Arena.h"
#include "GameLogic/ArenaTickAnimations.h"
#include "GameLogic/FighterTickAnimations.h"
#include "GameLogic/FighterCollisions.h"
#include "GameLogic/UI.h"
#include "GameLogic/FighterSprite.h"
#include <set>

#include <chrono>
using namespace std::chrono;

#define TEST_STATES

#ifdef TEST_STATES

int main(int argc, char **argv) {
	al_init();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();

	int screenWidth = 400, screenHeight = 254;
	int windowWidth = 1920, windowHeight = 1080;
	al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_SUGGEST);
	ALLEGRO_DISPLAY *window = al_create_display(windowWidth, windowHeight);
	ALLEGRO_BITMAP* buffer = al_create_bitmap(screenWidth, screenHeight);
	int sx = windowWidth / screenWidth;
	int sy = windowHeight / screenHeight;
	int scale = min(sx, sy);
	int scaleW = screenWidth * scale;
	int scaleH = screenHeight * scale;
	int offsetX = (windowWidth - scaleW) / 2;
	int offsetY = (windowHeight - scaleH) / 2;
	milliseconds ms = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
		);
	al_install_keyboard();
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	char pressedKeys[512] = { 0 };

	ALLEGRO_EVENT currentKey;
	AnimationFilmHolder &filmHolder = AnimationFilmHolder::Get();
	readBitmapConfig("assets/sprites/subzero/allFilms.json");
	readBitmapConfig("assets/sprites/scorpion/allFilms.json");
	readBitmapConfig("assets/UI.json");
	readFrameListAnimationsConfig("assets/animations/frameList.json");
	readFrameRangeAnimationsConfig("assets/animations/frameRange.json");
	readMovingPathAnimationsConfig("assets/animations/movingPath.json");
	readMovingAnimationsConfig("assets/animations/moving.json");

	milliseconds prev_ms = ms;
	timestamp_t ticks = ms.count();
	Fighter *f1_p, *f2_p;
	Arena *ar;
	SpritesHolder s;
	/* Construct GIM */
	GeneralInputManager gim(0);
	createGIMInputController(&gim, "assets/inputPlayer1.json", "assets/inputPlayer2.json");
	createGIMTransitions(&gim, &ticks);

	/* Construct Menus */
	Menu menu = Menu("assets/Backgrounds/Menus/menu.json", "assets/Backgrounds/Menus/menu_config.json", true, "menu");
	Menu pretitle = Menu("assets/Backgrounds/TitleScreen/pretitle.json", "assets/Backgrounds/TitleScreen/pretitle_config.json", true, "pretitle");
	Menu title = Menu("assets/Backgrounds/TitleScreen/titlescreen.json", "assets/Backgrounds/TitleScreen/titlescreen_config.json", true, "titlescreen");
	Menu select = Menu("assets/Backgrounds/CharacterSelect/character_select.json", "assets/Backgrounds/CharacterSelect/character_select_config.json", false, "select");
	Menu arena = Menu("assets/Backgrounds/ArenaSelect/arena_select.json", "assets/Backgrounds/ArenaSelect/arena_select_config.json", true, "arena_select");
	menu.addSpritesToHolder();
	pretitle.addSpritesToHolder();
	title.addSpritesToHolder();
	select.addSpritesToHolder();
	arena.addSpritesToHolder();
	gim.setCurrentMenu(&pretitle);

	/* Construct Arenas */
	std::string arena_sprites, arena_config;
	TickAnimation courtanim(500, 0, "courtAnim");
	TickAnimation throneanim(500, 0, "throneAnim");
	if (gim.getArenaSelection() == "courtyard") {
		arena_sprites = "assets/Backgrounds/Courtyard/courtyard.json";
		arena_config = "assets/Backgrounds/Courtyard/courtyard_config.json";
	}
	else if (gim.getArenaSelection() == "throneroom") {
		arena_sprites = "assets/Backgrounds/ThroneRoom/throneroom.json";
		arena_config = "assets/Backgrounds/ThroneRoom/throneroom_config.json";
	}
	Arena courtyard = Arena("assets/Backgrounds/Courtyard/courtyard.json", "assets/Backgrounds/Courtyard/courtyard_config.json", "courtyard");
	Arena throneroom = Arena("assets/Backgrounds/ThroneRoom/throneroom.json", "assets/Backgrounds/ThroneRoom/throneroom_config.json", "throneroom");
	courtyard.setTickAnimation(&courtanim);
	throneroom.setTickAnimation(&throneanim);
	courtanim.setTickFunc<std::function<void(void)>>(std::bind(&courtyardTickFunc, &courtyard));
	throneanim.setTickFunc<std::function<void(void)>>(std::bind(&throneroomTickFunc, &throneroom, &f1_p, &f2_p));

	/* Construct Fighters */
	AnimationsHolder::flipAllAndAdd("_inverted");
	FighterSprite f1_sprite = FighterSprite(0, 0, 10, nullptr, nullptr, HFrameAlign::LEFT, VFrameAlign::BOTTOM, "fighter", "noState");
	FighterSprite f2_sprite = FighterSprite(0, 0, 10, nullptr, nullptr, HFrameAlign::RIGHT, VFrameAlign::BOTTOM, "fighter", "noState");
	

	AudioHolder::loadAudio("assets/sounds.json");

	FighterCollisionHandler c = FighterCollisionHandler();

	MovingPathAnimator bloodAnim1, bloodAnim2;
	Sprite bloodSprite1 = Sprite(0, 0, f1_sprite.getZOrder() + 1, nullptr, AnimationFilmHolder::Get().GetFilm("blood"),
		HFrameAlign::LEFT, VFrameAlign::TOP, "blood", "noState");
	Sprite bloodSprite2 = Sprite(0, 0, f1_sprite.getZOrder() + 1, nullptr, AnimationFilmHolder::Get().GetFilm("blood"),
		HFrameAlign::LEFT, VFrameAlign::TOP, "blood", "noState");
	bloodSprite1.setVisibility(false);
	bloodSprite2.setVisibility(false);

	s.Add(&bloodSprite1);
	s.Add(&bloodSprite2);
	bool cIsSet = false;
	UI *ui = nullptr;
	/* Title Screen Game Loop */
	ViewWindow viewWin = ViewWindow(0, 0, screenWidth, screenHeight, 400, 254);
	while (true) {
		AudioHolder::playRepeatedSound("menu");
		while (true) {
			ms = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
				);
			ticks = ms.count();
			if (ms.count() - prev_ms.count() < 50) {
				if (al_get_next_event(event_queue, &currentKey)) {
					if (currentKey.type == ALLEGRO_EVENT_KEY_DOWN) {
						pressedKeys[currentKey.keyboard.keycode]++;
					}
				}
			}
			else {
				InputController::handleAll(pressedKeys);
				gim.getStateTransitions().setState(gim.getState());
				gim.processReceivedInput(true);
				//st.performTransitions(ic1.getLogical(a), true);
				prev_ms = ms;
				for (int i = 0; i < 512; i++)
					pressedKeys[i] = 0;
			}
			al_set_target_bitmap(buffer);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_clear_to_color(al_map_rgb(0, 0, 0));
			if (gim.getStateTransitions().getState() == "pretitle") {
				s.displayType(&viewWin, "pretitle");
			}
			else if (gim.getStateTransitions().getState() == "titlescreen") {
				gim.setCurrentMenu(&title);
				s.displayType(&viewWin, "titlescreen");
			}
			else if (gim.getStateTransitions().getState() == "arena" || gim.getStateTransitions().getState() == "courtyard" || gim.getStateTransitions().getState() == "throneroom") {
				gim.setCurrentMenu(&arena);
				s.displayType(&viewWin, "arena_select");
			}
			else if (gim.getStateTransitions().getState() != "ready") {
				gim.setCurrentMenu(&select);
				s.displayType(&viewWin, "select");
			}
			else {
				break;
			}

			al_set_target_backbuffer(window);
			al_draw_scaled_bitmap(buffer, 0, 0, screenWidth, screenHeight, offsetX, offsetY, scaleW, scaleH, 0);
			al_flip_display();
		}

		/* Construct Arenas */

		if (gim.getArenaSelection() == "courtyard") {
			ar = &courtyard;
		}
		else if (gim.getArenaSelection() == "throneroom") {
			ar = &throneroom;
		}
		else {
			ar = &courtyard;
		}
		ar->addSpritesToHolder();
		(*ar).startTickAnimator(ticks);

		/* Construct ViewWindow */
		viewWin = ViewWindow((*ar).getWidth() / 2 - screenWidth / 2, (*ar).getHeight() - (*ar).getViewBorder(3) - screenHeight,
			screenWidth, screenHeight, (*ar).getWidth(), (*ar).getHeight());
		TickAnimation viewWinTickAnim = TickAnimation(2, 0, "viewWinTickAnimation");
		viewWin.setTickAnimation(&viewWinTickAnim);
		viewWinTickAnim.setTickFunc<std::function<void(void)>>(std::bind(&scrollTickFunc, &f1_p, &f2_p, &viewWin, ar));
		viewWin.startTickAnimator(ticks);

		/* Construct Fighters */
		Layer actionLayer = { (*ar).getWidth(), (*ar).getHeight(), (*ar).getBorder(0), (*ar).getBorder(2), (*ar).getBorder(1), (*ar).getBorder(3) };
		const AnimationFilm * idle_f1 = filmHolder.GetFilm(gim.getFighterSelection(0) + "_idle");
		const AnimationFilm * idle_f2 = filmHolder.GetFilm(gim.getFighterSelection(1) + "_idle");
		f1_sprite.setLayer(&actionLayer);
		f2_sprite.setLayer(&actionLayer);
		f1_sprite.setAnimationFilm(idle_f1);
		f2_sprite.setAnimationFilm(idle_f2);
		f1_sprite.setY((*ar).getHeight() - (*ar).getFighterPos(1) - f1_sprite.getFrameBox().h);
		f2_sprite.setY((*ar).getHeight() - (*ar).getFighterPos(3) - f2_sprite.getFrameBox().h);
		f1_sprite.setX((*ar).getFighterPos(0));
		f2_sprite.setX((*ar).getFighterPos(2));
		s.Add(&f1_sprite);
		s.Add(&f2_sprite);

		
		Fighter f1(gim.getFighterSelection(0), &f1_sprite, true);
		Fighter f2(gim.getFighterSelection(1), &f2_sprite, false);

		FighterTickAnimations player1_tickanimations(&f1, &f2, &ticks, &viewWin);
		FighterTickAnimations player2_tickanimations(&f2, &f1, &ticks, &viewWin);

		TickAnimation player1_tickanimation(150, 0, "player1_tickanimation");
		TickAnimation player2_tickanimation(150, 0, "player2_tickanimation");
		f1.setTickAnimation(&player1_tickanimation);
		f2.setTickAnimation(&player2_tickanimation);

		std::map<std::string, std::pair<std::function<void(Fighter*, struct FighterInfoFilePaths)>, std::function<void(FighterTickAnimations*)>>> NameToConstruct;

		NameToConstruct["subzero"] = { constructSubzero, &FighterTickAnimations::setUpSubzeroTickAnimation };
		NameToConstruct["scorpion"] = { constructScorpion, &FighterTickAnimations::setUpScorpionTickAnimation };
		
		FighterInfoFilePaths player1paths = { "assets/inputPlayer1.json", "assets/animations/" + gim.getFighterSelection(0) + "_animationInfo.json",
			"_inverted", "assets/animations/" + gim.getFighterSelection(0) + "_basicInfo.json", "assets/animations/commonStrikes.json",
				"assets/animations/" + gim.getFighterSelection(0) + "_specialStrikes.json" };
		FighterInfoFilePaths player2paths = { "assets/inputPlayer2.json", "assets/animations/" + gim.getFighterSelection(1) + "_animationInfo.json",
			"_inverted", "assets/animations/" + gim.getFighterSelection(1) + "_basicInfo.json", "assets/animations/commonStrikes.json",
			"assets/animations/" + gim.getFighterSelection(1) + "_specialStrikes.json" };
		NameToConstruct[gim.getFighterSelection(0)].first(&f1, player1paths);
		NameToConstruct[gim.getFighterSelection(0)].second(&player1_tickanimations);
		NameToConstruct[gim.getFighterSelection(1)].first(&f2, player2paths);
		NameToConstruct[gim.getFighterSelection(1)].second(&player2_tickanimations);


		player1_tickanimation.setTickFunc<std::function<void(void)>>(std::bind(&FighterTickAnimations::onTickFunction, player1_tickanimations));
		f1.startTickAnimator(ticks);
		player2_tickanimation.setTickFunc<std::function<void(void)>>(std::bind(&FighterTickAnimations::onTickFunction, player2_tickanimations));
		f2.startTickAnimator(ticks);
		if (ui == nullptr) ui = new UI(&viewWin, ar, &f1, &f2, &ticks);
		else {
			ui->setFighters(&f1, &f2);
		}
		std::set<int> keysHeldPressed = {};
		std::set<int>  checkKeysForPressed = { f1.getInputController().getKeycode("down"), f1.getInputController().getKeycode("forward"),
			f1.getInputController().getKeycode("back"), f1.getInputController().getKeycode("block"),
			f1.getInputController().getKeycode("up"), f2.getInputController().getKeycode("down"), f2.getInputController().getKeycode("forward"),
			f2.getInputController().getKeycode("back"), f2.getInputController().getKeycode("block"),
			f2.getInputController().getKeycode("up") };
		
		

		bloodSprite1.setLayer(f1_sprite.getLayer());
		bloodSprite2.setLayer(f1_sprite.getLayer());
		if (!cIsSet) {
			c.bloodAnimator1 = &bloodAnim1;
			c.bloodSprite1 = &bloodSprite1;
			c.bloodAnimator2 = &bloodAnim2;
			c.bloodSprite2 = &bloodSprite2;
			c.currTime = &ticks;
			c.f1 = &f1;
			c.f2 = &f2;
			cIsSet = true;
		}
		else {
			ar->reRegister();
			AnimatorHolder::Register(&bloodAnim1);
			AnimatorHolder::Register(&bloodAnim2);
		}
		
		f1_sprite.addCollisionHandler(c);
		CollisionChecker::Get().registerPair(&f1_sprite, &f2_sprite, true);
		f1_p = &f1;
		f2_p = &f2;
		f1_sprite.setOtherFighterSprite(&f2_sprite);
		f1_sprite.setCollisionOffset(7);
		f2_sprite.setOtherFighterSprite(&f1_sprite);
		f2_sprite.setCollisionOffset(7);
		int round = 0, draws = 0;
		/* Game Loop */
		if (!f1.isOnLeftSide()) {
			f1.flipSide();
			f2.flipSide();
		}
		AudioHolder::stopPlayingSound("menu");
		ui->setArenaName(gim.getArenaSelection());
		while (true) {
			if (f1.getWins() == 2 || f2.getWins() == 2 || draws == 2) break;
			ui->stopWins();
			ui->displayRound(&ticks, round);
			gim.setState("running");
			gim.getStateTransitions().setState("running");
			for (int i = 0; i < 512; i++)
				pressedKeys[i] = 0;
			prev_ms = ms;


			while (!ui->winnerIsDisplayed()) {
				ms = duration_cast<milliseconds>(
					system_clock::now().time_since_epoch()
					);
				ticks = ms.count();
				if (ms.count() - prev_ms.count() < 150) {
					if (al_get_next_event(event_queue, &currentKey)) {
						if (currentKey.type == ALLEGRO_EVENT_KEY_DOWN) {
							pressedKeys[currentKey.keyboard.keycode]++;
							if (checkKeysForPressed.find(currentKey.keyboard.keycode) != checkKeysForPressed.end())
								keysHeldPressed.insert(currentKey.keyboard.keycode);
						}
						else if (currentKey.type == ALLEGRO_EVENT_KEY_UP && checkKeysForPressed.find(currentKey.keyboard.keycode) != checkKeysForPressed.end()) {
							if (keysHeldPressed.find(currentKey.keyboard.keycode) != keysHeldPressed.end())
								keysHeldPressed.erase(keysHeldPressed.find(currentKey.keyboard.keycode));
						}
					}
				}
				else {
					InputController::handleAll(pressedKeys);
					if (gim.getState() == "running") {
						f1.processReceivedInput(true);
						f2.processReceivedInput(true);
					}
					gim.getStateTransitions().setState(gim.getState());
					gim.processReceivedInput(true);
					//st.performTransitions(ic1.getLogical(a), true);
					prev_ms = ms;
					for (int i = 0; i < 512; i++)
						pressedKeys[i] = 0;
					for (auto keyCode : keysHeldPressed)
						pressedKeys[keyCode] = 1;
				}

				if (gim.getState() == "running") {
					gim.setCurrentMenu(&menu);
					AnimatorHolder::progress(ticks);
					f1_sprite.emergencyDisentangle();
					f2_sprite.emergencyDisentangle();
					CollisionChecker::Get().check();
					//else {
					if (f1.getHP() <= 0) {
						if (!ui->winnerIsBeingDisplayed()) {
							if (f1.getLosses() == 1 && !ui->fatality()) {
								ui->displayFinishHim(&f1);
							}
							else if (f1.getState() == "dead" || f1.getState() == "deadTripped" || f1.getState() == "deadBurned" || f1.getState() == "deadDecapitated") {
								f2.setNextAction("victory");
								f2.setState("victory");
								f2.resetY();
								f2.getCurrentlyRunningAnimator()->stop(false);
								f2.setIgnoreInput(true);
								ui->displayWinner(&f2);
								f2.incWins();
								round++;
							}
							f1.setIgnoreInput(true);
						}
					}
					else if (f2.getHP() <= 0) {
						if (!ui->winnerIsBeingDisplayed()) {
							if (f2.getLosses() == 1 && !ui->fatality()) {
								ui->displayFinishHim(&f2);
							}
							else if (f2.getState() == "dead" || f2.getState() == "deadTripped" || f2.getState() == "deadBurned" || f2.getState() == "deadDecapitated") {
								f1.setNextAction("victory");
								f1.setState("victory");
								f1.resetY();
								f1.getCurrentlyRunningAnimator()->stop(false);
								f1.setIgnoreInput(true);
								ui->displayWinner(&f1);
								f1.incWins();
								round++;
							}
							f2.setIgnoreInput(true);
						}
					}
					else if (ui->timerHasExpired() && !ui->winnerIsBeingDisplayed()) {
						if (f1.getHP() > f2.getHP()) {
							f1.getCurrentlyRunningAnimator()->stop(false);
							f2.getCurrentlyRunningAnimator()->stop(false);
							f1.incWins();
							ui->displayWinner(&f1);
						}
						else if (f2.getHP() > f1.getHP()) {
							f1.getCurrentlyRunningAnimator()->stop(false);
							f2.getCurrentlyRunningAnimator()->stop(false);
							f2.incWins();
							ui->displayWinner(&f2);
						}
						else {
							f1.getCurrentlyRunningAnimator()->stop(false);
							f2.getCurrentlyRunningAnimator()->stop(false);
							draws++;
							ui->displayDraw();
						}
						round++;
						f1.setIgnoreInput(true);
						f2.setIgnoreInput(true);
					}

					al_set_target_bitmap(buffer);
					al_clear_to_color(al_map_rgb(0, 0, 0));
					s.displayRunning(&viewWin, ar->getName());
					al_set_target_backbuffer(window);
					al_draw_scaled_bitmap(buffer, 0, 0, screenWidth, screenHeight, offsetX, offsetY, scaleW, scaleH, 0);
					al_flip_display();
				}
				else {
					al_set_target_bitmap(buffer);
					al_clear_to_color(al_map_rgb(0, 0, 0));
					if (gim.getStateTransitions().getState() == "Screenshot") {
						s.displayRunning(&viewWin, ar->getName());
					}
					else {
						gim.setCurrentMenu(&menu);
						s.displayType(&viewWin, "menu");
					}
					al_set_target_backbuffer(window);
					al_draw_scaled_bitmap(buffer, 0, 0, screenWidth, screenHeight, offsetX, offsetY, scaleW, scaleH, 0);
					al_flip_display();
				}
			}
			gim.setState("ready");
			gim.getStateTransitions().setState("ready");
		}
		gim.setState("select00");
		gim.getStateTransitions().setState("select00");
		gim.setCurrentMenu(&select);
		gim.resetGIM();
		AnimatorHolder::cancelAll();
		ar->removeSpritesFromHolder();
		s.Remove(&f1_sprite);
		s.Remove(&f2_sprite);
		f1.getSprite()->setFlip(false);
		f2.getSprite()->setFlip(false);
	}
}

#endif