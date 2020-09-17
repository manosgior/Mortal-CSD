#include "configReaders.h"
#include "json-develop/include/nlohmann/json.hpp"
#include <fstream>
#include <map>
#include <iostream>

using json = nlohmann::json;

void readBitmapConfig(std::string filepath) {
	std::ifstream inputFile;
	inputFile.open(filepath, std::ifstream::in);
	assert(!(inputFile.fail()));
	json spritesheet;
	AnimationFilmHolder &holder = AnimationFilmHolder::Get();
	std::vector<std::string> allIds;
	std::vector<std::string> allPaths;
	std::vector<std::vector<Rect>> allBoxes;
	while(!inputFile.eof()){
		inputFile >> spritesheet;
		inputFile.peek();
		allIds.push_back(spritesheet["id"].get<std::string>());
		allPaths.push_back(spritesheet["filepath"].get<std::string>());
		std::vector<Rect> currentBoxes;
		for (auto boxJson : spritesheet["boxes"]) {
			Rect box = { boxJson[0], boxJson[1], boxJson[2], boxJson[3] };
			currentBoxes.push_back(box);
		}
		allBoxes.push_back(currentBoxes);
	}
	holder.Load(allIds, allPaths, allBoxes);
}

MovingAnimation* makeMovingAnimation(json animationInfo);
FrameRangeAnimation* makeFrameRangeAnimation(json animationInfo);
FrameListAnimation* makeFrameListAnimation(json animationInfo);
FlashAnimation* makeFlashAnimation(json animationInfo);
ScrollAnimation* makeScrollAnimation(json animationInfo);
MovingPathAnimation* makeMovingPathAnimation(json animationInfo);

void readMovingAnimationsConfig(std::string filepath) {
	std::ifstream inputFile;
	inputFile.open(filepath, std::ifstream::in);
	assert(!(inputFile.fail()));
	json animationInfo;
	while (!inputFile.eof()) {
		inputFile >> animationInfo;
		inputFile.peek();
		AnimationsHolder::addMovingAnimation(makeMovingAnimation(animationInfo));
	}
}

void readFrameRangeAnimationsConfig(std::string filepath) {
	std::ifstream inputFile;
	inputFile.open(filepath, std::ifstream::in);
	assert(!(inputFile.fail()));
	json animationInfo;
	while (!inputFile.eof()) {
		inputFile >> animationInfo;
		inputFile.peek();
		AnimationsHolder::addFrameRangeAnimation(makeFrameRangeAnimation(animationInfo));
	}
}

void readFrameListAnimationsConfig(std::string filepath) {
	std::ifstream inputFile;
	inputFile.open(filepath, std::ifstream::in);
	assert(!(inputFile.fail()));
	json animationInfo;
	while (!inputFile.eof()) {
		inputFile >> animationInfo;
		inputFile.peek();
		AnimationsHolder::addFrameListAnimation(makeFrameListAnimation(animationInfo));
	}
}

void readScrollAnimationsConfig(std::string filepath) {
	std::ifstream inputFile;
	inputFile.open(filepath, std::ifstream::in);
	assert(!(inputFile.fail()));
	json animationInfo;
	while (!inputFile.eof()) {
		inputFile >> animationInfo;
		inputFile.peek();
		AnimationsHolder::addScrollAnimation(makeScrollAnimation(animationInfo));
	}
}

void readFlashAnimationsConfig(std::string filepath) {
	std::ifstream inputFile;
	inputFile.open(filepath, std::ifstream::in);
	assert(!(inputFile.fail()));
	json animationInfo;
	while (!inputFile.eof()) {
		inputFile >> animationInfo;
		inputFile.peek();
		AnimationsHolder::addFlashAnimation(makeFlashAnimation(animationInfo));
	}
}

void readMovingPathAnimationsConfig(std::string filepath) {
	std::ifstream inputFile;
	inputFile.open(filepath, std::ifstream::in);
	assert(!(inputFile.fail()));
	json animationInfo;
	while (!inputFile.eof()) {
		inputFile >> animationInfo;
		inputFile.peek();
		AnimationsHolder::addMovingPathAnimation(makeMovingPathAnimation(animationInfo));
	}
}

MovingAnimation *makeMovingAnimation(json animationInfo) {
	return new MovingAnimation(animationInfo["dx"].get<offset_t>(), animationInfo["dy"].get<offset_t>(),
		animationInfo["delay"].get<delay_t>(), animationInfo["continuous"].get<bool>(), animationInfo["id"].get<animid_t>());
}

FrameRangeAnimation *makeFrameRangeAnimation(json animationInfo) {
	return new FrameRangeAnimation(animationInfo["startFrame"].get<frame_t>(), animationInfo["endFrame"].get<frame_t>(), animationInfo["dx"].get<offset_t>(), 
		animationInfo["dy"].get<offset_t>(), animationInfo["delay"].get<delay_t>(), animationInfo["continuous"].get<bool>(), animationInfo["id"].get<animid_t>());
}

FrameListAnimation *makeFrameListAnimation(json animationInfo) {
	std::vector<frame_t> frames;
	for (auto frameIndex : animationInfo["frames"]) {
		frames.push_back(frameIndex.get<int>());
	}
	return new FrameListAnimation(frames, animationInfo["dx"].get<offset_t>(), animationInfo["dy"].get<offset_t>(), animationInfo["delay"].get<delay_t>(),
		animationInfo["continuous"].get<bool>(), animationInfo["id"].get<animid_t>());
}

FlashAnimation *makeFlashAnimation(json animationInfo) {
	return new FlashAnimation(animationInfo["repetitions"].get<frame_t>(), animationInfo["showDelay"].get<delay_t>(), 
		animationInfo["hideDelay"].get<delay_t>(), animationInfo["id"].get<animid_t>());
}

ScrollAnimation *makeScrollAnimation(json animationInfo) {
	std::vector<ScrollEntry> scrolls;
	for(auto scrollEntry : animationInfo["scrolls"]) {
		scrolls.push_back({ scrollEntry[0].get<offset_t>(), scrollEntry[1].get<offset_t>(), scrollEntry[2].get<delay_t>() });
	}
	return new ScrollAnimation(scrolls, animationInfo["id"].get<animid_t>(), animationInfo["continuous"].get<bool>());
}

MovingPathAnimation *makeMovingPathAnimation(json animationInfo) {
	std::vector<PathEntry> pathEntries;
	for (auto pathEntry : animationInfo["pathEntries"]) {
		pathEntries.push_back( PathEntry(pathEntry[0].get<offset_t>(), pathEntry[1].get<offset_t>(), pathEntry[2].get<frame_t>(), pathEntry[3].get<delay_t>()) );
	}
	return new MovingPathAnimation(pathEntries, animationInfo["id"].get<animid_t>(), animationInfo["continuous"].get<bool>());
}