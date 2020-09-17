#include "FighterSprite.h"


FighterSprite::FighterSprite(Dim _x, Dim _y, int _zOrder, Layer *layer, const AnimationFilm* film, HFrameAlign h, VFrameAlign v, std::string type, std::string state) :
	Sprite(_x, _y, _zOrder, layer, film, h, v, type, state){
	noClip = false;
	beingPushed = false;
}

void FighterSprite::setLayer(Layer * l) {
	Sprite::setLayer(l);
	borders[0] = l->border_west;
	borders[1] = l->border_north;
	borders[2] = l->width - l->border_east;
	borders[3] = l->height - l->border_south;
}

void FighterSprite::setFrame(int i)  {
	int sx = this->x, myW = this->frameBox.w;
	int sy = this->y, myH = this->frameBox.h;

	Sprite::setFrame(i);
	sx = this->x, myW = this->frameBox.w;
	sy = this->y, myH = this->frameBox.h;

	if (sx < borders[0]) {
		this->x = borders[0];
	}
	else if(sx + myW> borders[2]){
		this->x = borders[2] - myW;
	}

	if (sy < borders[1]) {
		this->y = borders[1];
	}
	else if (sy + frameBox.h > borders[3]) {
		this->y = borders[3] - myH;
	}

	assert(this->x >= borders[0]);
	assert(this->x + myW <= borders[2]);
}

void FighterSprite::move(int dx, int dy) {
	int sx = this->x, myW = this->frameBox.w;
	int sy = this->y, myH = this->frameBox.h;
	int colPad = 0;
	if (dx == 0 && dy == 0)
		return;

	if (!beingPushed && (state == "neu" || state == "def" | state == "jmp") && 
		(otherFighterSprite->state == "neu" || otherFighterSprite->state == "def" || otherFighterSprite->state == "jmp")) {

		int otherX = otherFighterSprite->x, otherY = otherFighterSprite->y, 
			otherW = otherFighterSprite->frameBox.w, otherH = otherFighterSprite->frameBox.h;
		bool willCollide = !((otherX + otherW - colPad < sx + dx + colPad) || (sx + dx + myW - colPad < otherX + colPad) ||
			(otherY + otherH - colPad < sy + dy + colPad) || (sy + dy + myH - colPad < otherY + colPad));

		if (willCollide) {
			otherFighterSprite->beingPushed = true;
			//pushing the other player while i am standing, make sure he is on my other side
			if (dy == 0 && otherFighterSprite->state != "jmp" && ((dx > 0 && sx <= otherX) || (dx < 0 && sx >= otherX)) ) {
				otherFighterSprite->move(dx, 0);
				dx = dx / 2;
			}
			//if we're both jumping (and he's on my other side)
			else if (((dx > 0 && sx <= otherX) || (dx < 0 && sx >= otherX)) && otherFighterSprite->state == "jmp" && state == "jmp") {
				//otherFighterSprite->move(-dx, 0);
				dx = 0;
			}
			//move the other player while i am jumping, make sure he is on my other side
			else if (dx != 0 && state == "jmp" && otherFighterSprite->state != "jmp"){
				int moveStep = (dx > 0) ? (5) : (-5);
				int moveBy = (dy < 0)?(moveStep):(-moveStep);

				while (willCollide) {
					if (!otherFighterSprite->canMove(moveBy, 0))
						moveBy = -moveBy;
					else
						otherFighterSprite->move(moveBy, 0);
					otherX = otherFighterSprite->x;
					otherY = otherFighterSprite->y;
					otherW = otherFighterSprite->frameBox.w;
					otherH = otherFighterSprite->frameBox.h;
					willCollide = !((otherX + otherW - 15 < sx + dx) || (sx + dx + myW - 15 < otherX) ||
						(otherY + otherH - 15 < sy + dy) || (sy + dy + myH - 15 < otherY));
				}
			}
			otherFighterSprite->beingPushed = false;

		}
		//check if the other sprite managed to move
		otherX = otherFighterSprite->x;
		otherY = otherFighterSprite->y;
		otherW = otherFighterSprite->frameBox.w;
		otherH = otherFighterSprite->frameBox.h;
		willCollide = !((otherX + otherW - 15 < sx + dx) || (sx + dx + myW - 15 < otherX) ||
			(otherY + otherH - 15 < sy + dy) || (sy + dy + myH - 15 < otherY));
		if (willCollide) {
			dx = 0;
		}

	}

	if (!noClip && (sx + dx < borders[0] || sx + dx + myW> borders[2])) {
		dx = 0;
	}
	if (!noClip && (sy + dy < borders[1] || sy + dy + myH> borders[3])) {
		dy = 0;
	}
	if (sy > borders[3]) {
		y = 0;
	}

	do {
		auto dxFinal(
			dx >= 0 ? min(GRID_SIZE, dx) : max(-GRID_SIZE, dx)
		);
		auto dyFinal(
			dy >= 0 ? min(GRID_SIZE, dy) : max(-GRID_SIZE, dy)
		);

		this->x += dxFinal;
		this->y += dyFinal;

		dx -= dxFinal;
		dy -= dyFinal;

	} while (dx || dy);
}

void FighterSprite::setNoClip(bool nc) {
	noClip = nc;
}

bool FighterSprite::getNoClip() {
	return noClip;
}

void FighterSprite::setOtherFighterSprite(FighterSprite *fs) {
	otherFighterSprite = fs;
}

void FighterSprite::setBorder(Dim i, Dim j) {
	borders[i] = j;
}

Dim FighterSprite::getBorder(Dim i) {
	return borders[i];
}

bool FighterSprite::canMove(int dx, int dy) {
	int sx = this->x, myW = this->frameBox.w;
	int sy = this->y, myH = this->frameBox.h;

	return !( (sx + dx < borders[0] || sx + dx > borders[2] - frameBox.w) ||
		(sy + dy < borders[1] || sy + dy > borders[3] - frameBox.h) || (sy > borders[3]) );
}

void FighterSprite::emergencyDisentangle() {
	if (this->collisionCheck(otherFighterSprite, false) && state != "atk" && otherFighterSprite->state != "atk") {
		if (x < otherFighterSprite->x) {
			while (collisionCheck(otherFighterSprite, false)) {
				if (canMove(-5, 0)) {
					beingPushed = true;
					move(-5, 0);
					beingPushed = false;
				}
				else {
					otherFighterSprite->beingPushed = true;
					otherFighterSprite->move(5, 0);
					otherFighterSprite->beingPushed = false;
				}
			}
		}
	}
}