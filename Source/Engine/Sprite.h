#pragma once

#include <allegro5/allegro.h>
#include <list>
#include <set>
#include "GTypes.h"
#include "AnimationFilm.h"
#include "ViewWindow.h"


class Sprite;

struct CollisionHandler {
	virtual void operator()(Sprite *caller, Sprite *arg) = 0;
	virtual CollisionHandler *clone() const = 0;
	virtual ~CollisionHandler() { }
};


class Sprite {
private:
	int						frameNo;
	int						zOrder;
	bool					visible;
	const AnimationFilm*	currFilm;
	Layer*					myLayer;
	HFrameAlign				horAlign;
	VFrameAlign				verAlign;
	bool					drawFlipped;
	std::set<CollisionHandler*> markedForRemoval;
	int						collisionOffset = 0;

protected:
	Dim						x, y;
	Rect					frameBox;
	std::string				type;
	std::string				state;
	typedef std::list<CollisionHandler*> Handlers;
	Handlers handlers;
	void NotifyCollision(Sprite *s);
public:
	Sprite(Dim _x, Dim _y, int zOrder, Layer *layer, const AnimationFilm* film, HFrameAlign h, VFrameAlign v, std::string type, std::string state);
	~Sprite();

	virtual void setFrame(int i);
	int getFrame() const;

	Rect getFrameBox();

	void setX(Dim);
	void setY(Dim);
	Dim getX();
	Dim getY();
	int getZOrder();
	void setZOrder(int);

	void setVisibility(bool v);
	bool isVisible(void) const;

	void setAnimationFilm(const AnimationFilm *film);
	const AnimationFilm* getFilm();
	
	void setLayer(Layer * l);
	Layer* getLayer();

	void setHAlign(HFrameAlign);
	HFrameAlign getHAlign();
	void setVAlign(VFrameAlign);
	VFrameAlign getVAlign();

	std::string getType();

	void setState(std::string state);
	std::string getState();

	virtual void move(int dx, int dy);
	void flip();
	void setFlip(bool);
	void display(const ViewWindow *view);
	
	void clearHandlers();
	void clearMarked();
	bool collisionCheck(Sprite *s, bool alsoCallHandlers);

	void addCollisionHandler(const CollisionHandler& h) {
		handlers.push_back(h.clone());
	}

	void markCollisionHandlerForRemoval(CollisionHandler* h) {
		markedForRemoval.insert(h);
	}
	void setCollisionOffset(int c);
};