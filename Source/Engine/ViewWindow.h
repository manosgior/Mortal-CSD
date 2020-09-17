#pragma once
#include "GTypes.h"
#include "../Engine/TickAnimation.h"
#include "../Engine/TickAnimator.h"
#include "../Engine/AnimatorHolder.h"


class ViewWindow {
	Dim x, y, w, h, actionLayerWidth, actionLayerHeight;
	Dim initialX;
	Dim initialY;
	TickAnimation *tickAnimation;
	TickAnimator tickAnimator;
public:
	ViewWindow(Dim _x, Dim _y, Dim _w, Dim _h, Dim _actionLayerWidth, Dim _actionLayerHeight);
	void resetInitialCoordinates();
	Dim getX() const;
	Dim getY() const;
	Dim getWidth() const;
	Dim getHeight() const;
	Dim getActionLayerWidth() const;
	Dim getActionLayerHeight() const;
	bool canHorScroll(short dx) const;
	bool canVerScroll(short dy) const;
	void scroll(short dx, short dy);
	void setTickAnimation(TickAnimation*);
	void startTickAnimator(timestamp_t);
};