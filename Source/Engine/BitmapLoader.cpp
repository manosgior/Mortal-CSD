#include "BitmapLoader.h"
#include <iterator>
#include <iostream>

Bitmap_t BitmapLoader::GetBitmap(const std::string& path) const {
	Bitmaps::const_iterator  i = bitmap.find(path);
	return i != bitmap.end() ? i->second : nullptr;
}

Bitmap_t BitmapLoader::Load(const std::string& path) {
	Bitmap_t b = GetBitmap(path);

	if (b == nullptr) {
		//std::cout << "reading\t" << path.c_str() << std::endl;
		b = al_load_bitmap(path.c_str());
		al_convert_mask_to_alpha(b, al_map_rgb(255, 0, 255));
		bitmap[path] = b;
		assert(b);
	}
	return b;
}

void BitmapLoader::CleanUp() {
	for (Bitmaps::iterator i = bitmap.begin(); i != bitmap.end(); i++) {
		al_destroy_bitmap(i->second);
	}
	bitmap.clear();
}

BitmapLoader::BitmapLoader() {}
BitmapLoader::~BitmapLoader() {}