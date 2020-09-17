#pragma once

#include "GTypes.h"
#include<allegro5/allegro.h>
#include <string>
#include <vector>
#include "ViewWindow.h"

class AnimationFilm {
private:
	std::vector<Rect>		boundingBoxes;
	Bitmap_t				bitmap;
	std::string				id;
public:
	AnimationFilm(Bitmap_t bmp, std::vector<Rect> bboxes, std::string id);

	int						getTotalFrames() const;
	const Bitmap_t			getBitmap() const;
	const std::string		GetId() const;
	const Rect				getFrameBox(int frameNo) const;
	void					displayFrame(const ViewWindow *view, const Layer *layer, const Point& at, int frameNo, bool drawFlipped) const;
};