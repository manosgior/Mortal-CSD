#include "AnimationFilm.h"
#include <assert.h>

AnimationFilm::AnimationFilm(Bitmap_t bmp, std::vector<Rect> bboxes, std::string id) {
	this->id = id;
	boundingBoxes = bboxes;
	bitmap = bmp;
}

int	AnimationFilm::getTotalFrames() const{
	return boundingBoxes.size();
}

const Bitmap_t AnimationFilm::getBitmap() const {
	return bitmap;
}

const std::string AnimationFilm::GetId() const {
	return id;
}

const Rect AnimationFilm::getFrameBox(int frameNo) const {
	assert(frameNo < boundingBoxes.size());
	return boundingBoxes[frameNo];
}

bool isClippedEntirely(Dim viewX, Dim viewY, Dim viewW, Dim viewH, const Point& at, const Rect &box) {
	return ((viewX + viewW < at.x) || (at.x + box.w < viewX) || (viewY + viewH < at.y) || (at.y + box.h < viewY));
}
#include <iostream>
void AnimationFilm::displayFrame(const ViewWindow *view, const Layer *layer, const Point& at, int frameNo, bool drawFlipped) const {
	//al_set_target_backbuffer(drawTo);
	//clipping and appropriate offsetting
	Dim transformedViewX = view->getX();
	Dim transformedViewY = view->getY();
	int flags = (drawFlipped) ? ALLEGRO_FLIP_HORIZONTAL : 0;
	if(view->getActionLayerWidth() != view->getWidth())
		transformedViewX = (layer->width - view->getWidth())*view->getX() / (view->getActionLayerWidth() - view->getWidth());
	if(view->getActionLayerHeight() != view->getHeight())
		transformedViewY = (layer->height - view->getHeight())*view->getY() / (view->getActionLayerHeight() - view->getHeight());
	if (!isClippedEntirely(transformedViewX, transformedViewY, view->getWidth(), view->getHeight(), at, boundingBoxes[frameNo])) {
		Dim clippedRectX = max(transformedViewX, at.x), clippedRectY = max(transformedViewY, at.y);
		Dim clippedRectWidth = min(transformedViewX + view->getWidth(), at.x + boundingBoxes[frameNo].w) - clippedRectX;
		Dim clippedRectHeight = min(transformedViewY + view->getHeight(), at.y + boundingBoxes[frameNo].h) - clippedRectY;
		Dim xOffset = clippedRectX - at.x, yOffset = clippedRectY - at.y;
		if (drawFlipped) {
			Bitmap_t tempBitmap = al_create_bitmap(boundingBoxes[frameNo].w, boundingBoxes[frameNo].h);
			Bitmap_t old = al_get_target_bitmap();
			al_set_target_bitmap(tempBitmap);
			//flip it
			al_draw_bitmap_region(bitmap, boundingBoxes[frameNo].x, boundingBoxes[frameNo].y,
				boundingBoxes[frameNo].w, boundingBoxes[frameNo].h, 0, 0, ALLEGRO_FLIP_HORIZONTAL);

			al_set_target_bitmap(old);
			//clip it
			al_draw_bitmap_region(tempBitmap, xOffset, yOffset,
				clippedRectWidth, clippedRectHeight, clippedRectX - transformedViewX, clippedRectY - transformedViewY, 0);
			//whip it good
			al_destroy_bitmap(tempBitmap);
		}
		else {
			al_draw_bitmap_region(bitmap, xOffset + boundingBoxes[frameNo].x, yOffset + boundingBoxes[frameNo].y,
				clippedRectWidth, clippedRectHeight, clippedRectX - transformedViewX, clippedRectY - transformedViewY, 0);
		}

	}
}