#pragma once
#include <string>
#include "../Engine/AnimationFilmHolder.h"
#include "../Engine/Animation.h"
#include "../Engine/MovingAnimation.h"
#include "../Engine/FrameRangeAnimation.h"
#include "../Engine/FrameListAnimation.h"
#include "../Engine/MovingPathAnimation.h"
#include "../Engine/FlashAnimation.h"
#include "../Engine/ScrollAnimation.h"
#include "../Engine/AnimationsHolder.h"

void readBitmapConfig(std::string filepath);

void readMovingAnimationsConfig(std::string filepath);

void readFrameRangeAnimationsConfig(std::string filepath);

void readFrameListAnimationsConfig(std::string filepath);

void readScrollAnimationsConfig(std::string filepath);

void readFlashAnimationsConfig(std::string filepath);

void readMovingPathAnimationsConfig(std::string filepath);
