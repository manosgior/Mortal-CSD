#pragma once

#include <map>
#include "GTypes.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

class BitmapLoader {
private:
	typedef std::map<std::string, ALLEGRO_BITMAP *> Bitmaps;
	Bitmaps bitmap;
	Bitmap_t GetBitmap(const std::string& path) const;
public:
	Bitmap_t Load(const std::string& path);
	void CleanUp();
	BitmapLoader();
	~BitmapLoader();
};