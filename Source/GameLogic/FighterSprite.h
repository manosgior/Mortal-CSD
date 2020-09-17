#pragma once
#include "../Engine/Sprite.h"


class FighterSprite : public Sprite {
private:
	Dim						borders[4];
	bool					noClip;
	FighterSprite*			otherFighterSprite;
	bool					beingPushed;
	bool					canMove(int dx, int dy);

public:
	FighterSprite(Dim _x, Dim _y, int _zOrder, Layer *layer, const AnimationFilm* film, HFrameAlign h, VFrameAlign v, std::string type, std::string state);
	virtual void move(int dx, int dy) override;
	virtual void setFrame(int i) override;
	void setNoClip(bool nc);
	bool getNoClip();
	void setLayer(Layer * l);
	void setOtherFighterSprite(FighterSprite *);
	void setBorder(Dim i, Dim j);
	Dim getBorder(Dim i);
	void emergencyDisentangle();
};