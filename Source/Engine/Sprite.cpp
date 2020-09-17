#include "Sprite.h"

#include <iostream>
#include <algorithm>

Sprite::Sprite(Dim _x, Dim _y, int _zOrder, Layer *layer, const AnimationFilm* film, HFrameAlign h, VFrameAlign v, std::string type, std::string state):
	x(_x), y(_y), zOrder(_zOrder), myLayer(layer), currFilm(film), visible(true), type(type), state(state) {
	if (film != nullptr) frameBox = film->getFrameBox(0);
	frameNo = 0;
	setFrame(0);
	horAlign = h;
	verAlign = v;
	drawFlipped = false;
}

void Sprite::setFrame(int i) {
	if (i != frameNo) {
		assert(i < currFilm->getTotalFrames());
		Rect oldBox = frameBox;
		frameBox = currFilm->getFrameBox(frameNo = i);
		switch (horAlign) {
			case HFrameAlign::LEFT:
				break;
			case HFrameAlign::CENTER:
				move((oldBox.w - frameBox.w)/2, 0);
				break;
			case HFrameAlign::RIGHT:
				move((oldBox.w - frameBox.w), 0);
				break;
			default:
				assert(0);
		}
		switch (verAlign) {
			case VFrameAlign::TOP:				
				break;
			case VFrameAlign::CENTER:
				move(0, (oldBox.h - frameBox.h) / 2);
				break;
			case VFrameAlign::BOTTOM:
				move(0, (oldBox.h - frameBox.h));
				break;
			default:
				assert(0);
		}
		
	}
}

int Sprite::getFrame() const {
	return frameNo;
}

void Sprite::setAnimationFilm(const AnimationFilm *film) {
	currFilm = film;
	frameNo = -1; //temporary, so that setFrame is guaranteed to spot a change
	setFrame(0);
}

void Sprite::setVisibility(bool v) {
	visible = v;
}

bool Sprite::isVisible() const {
	return visible;
}

void Sprite::move(int dx, int dy) {
	int sx = this->x, myW = this->frameBox.w;
	int sy = this->y, myH = this->frameBox.h;
	if (dx == 0 && dy == 0)
		return;

	do {	
		auto dxFinal(
			dx >= 0 ? min(GRID_SIZE, dx) : max(-GRID_SIZE, dx)
		);
		auto dyFinal(
			dy >= 0 ? min(GRID_SIZE, dy) : max(-GRID_SIZE, dy)
		);

		Sprite::x += dxFinal;
		Sprite::y += dyFinal;

		dx -= dxFinal;
		dy -= dyFinal;
		
	} while (dx || dy);
}

void Sprite::display(const ViewWindow *view) {
	Point p = { x, y };
	if (visible)
		currFilm->displayFrame(view, myLayer, p, frameNo, drawFlipped);
}

std::string Sprite::getType() {
	return type;
}

const AnimationFilm* Sprite::getFilm() {
	return currFilm;
}

std::string Sprite::getState() {
	return state;
}

void Sprite::setState(std::string state) {
	this->state = state;
}

void Sprite::NotifyCollision(Sprite *s) {
	for (Handlers::iterator i = handlers.begin(); i != handlers.end(); i++) {
		if (markedForRemoval.find(*i) == markedForRemoval.end())(**i)(this, s);
	}
}

void Sprite::clearHandlers() {
	handlers.clear();
}

void Sprite::clearMarked() {
	for (CollisionHandler* col : markedForRemoval) {
		handlers.remove(col);
	}
	markedForRemoval.clear();
}

bool Sprite::collisionCheck(Sprite *s, bool alsoCallHandlers) {
	int myX = this->x, myW = this->frameBox.w, myY = this->y, myH = this->frameBox.h;
	int otherX = s->getX(), otherW = s->getFrameBox().w, otherY = s->getY(), otherH = s->getFrameBox().h;
	if (!((otherX + otherW - s->collisionOffset < myX + collisionOffset) ||
		(myX + myW - collisionOffset < otherX + s->collisionOffset) ||
		(otherY + otherH - s->collisionOffset < myY + collisionOffset) ||
		(myY + myH - collisionOffset < otherY + s->collisionOffset))) {
		if (alsoCallHandlers)
			NotifyCollision(s);
		clearMarked();
		return true;
	}
	clearMarked();
	return false;
}

Sprite::~Sprite() {
	clearHandlers();
}

int Sprite::getZOrder() {
	return zOrder;
}

void Sprite::setZOrder(int order) {
	zOrder = order;
}

Rect Sprite::getFrameBox() {
	return frameBox;
}

Dim Sprite::getX() {
	return x;
}

Dim Sprite::getY() {
	return y;
}

void Sprite::setX(Dim _x) {
	x = _x;
}

void Sprite::setY(Dim _y) {
	y = _y;
}

void Sprite::setLayer(Layer * l) {
	myLayer = l;
}

Layer *Sprite ::getLayer() {
	return myLayer;
}

void Sprite::flip() {
	drawFlipped = !(drawFlipped);
}

void Sprite::setFlip(bool b) {
	drawFlipped = b;
}

void Sprite::setHAlign(HFrameAlign hAlign) {
	horAlign = hAlign;
}

HFrameAlign Sprite::getHAlign() {
	return horAlign;
}

void Sprite::setVAlign(VFrameAlign v) {
	verAlign = v;
}

VFrameAlign Sprite::getVAlign() {
	return verAlign;
}

void Sprite::setCollisionOffset(int c) {
	collisionOffset = c;
}