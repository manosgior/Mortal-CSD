#include "ViewWindow.h"


ViewWindow::ViewWindow(Dim _x, Dim _y, Dim _w, Dim _h, Dim _actionLayerWidth, Dim _actionLayerHeight) :
	x(_x), y(_y), initialX(_x), initialY(_y), actionLayerWidth(_actionLayerWidth), actionLayerHeight(_actionLayerHeight) {
	w = _w;
	if (x + w > actionLayerWidth)
		x = actionLayerWidth - w;
	h = _h;
	if (y + h > actionLayerHeight)
		y = actionLayerHeight - h;
}

void ViewWindow::resetInitialCoordinates() {
	x = initialX;
	y = initialY;
}

bool ViewWindow::canHorScroll(short dx) const {
	int tmp = x;
	if (tmp + dx >= 0 && tmp + w + dx < actionLayerWidth)
		return true;
	return false;
}

bool ViewWindow::canVerScroll(short dy) const {
	int tmp = y;
	if (tmp + dy >= 0 && tmp + h + dy < actionLayerHeight)
		return true;
	return false;
}

void ViewWindow::scroll(short dx, short dy) {
	int tmpX = x, tmpY = y;
	if (canHorScroll(dx))
		x = tmpX + dx;
	else if (dx < 0)
		x = 0;
	else
		x = actionLayerWidth - w;
	if (canVerScroll(dy))
		y = tmpY + dy;
	else if (dy < 0)
		y = 0;
	else
		y = actionLayerHeight - h;

}


Dim ViewWindow::getX() const{
	return x;
}

Dim ViewWindow::getY() const {
	return y;
}

Dim ViewWindow::getWidth() const {
	return w;
}

Dim ViewWindow::getHeight() const {
	return h;
}

Dim ViewWindow::getActionLayerWidth() const {
	return actionLayerWidth;
}

Dim ViewWindow::getActionLayerHeight() const {
	return actionLayerHeight;
}

void ViewWindow::setTickAnimation(TickAnimation* t) {
	tickAnimation = t;
}

void ViewWindow::startTickAnimator(timestamp_t currTime) {
	tickAnimator.start(tickAnimation, currTime);
	AnimatorHolder::markAsRunning(&tickAnimator);
}