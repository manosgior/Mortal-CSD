#include "AnimationFilmHolder.h"
#include <vector>

AnimationFilmHolder AnimationFilmHolder::holder = AnimationFilmHolder();

AnimationFilmHolder& AnimationFilmHolder::Get() {
	return holder;
}

void AnimationFilmHolder::Load(std::vector<std::string> ids, std::vector<std::string> paths, std::vector<std::vector<Rect>> rects){
	for (unsigned int i = 0; i < ids.size(); i++) {
		Bitmap_t bitmap = bitmapLoader.Load(paths[i]);
		assert(!GetFilm(ids[i]));
		films[ids[i]] = new AnimationFilm(bitmap, rects[i], ids[i]);
	}
}

void AnimationFilmHolder::CleanUp() {
	for (Films::iterator i = films.begin(); i != films.end(); i++) {
		delete(i->second);
	}
	films.clear();
}

const AnimationFilm* AnimationFilmHolder::GetFilm(const std::string id) const {
	Films::const_iterator i = films.find(id);
	return i != films.end() ? i->second : nullptr;
}

AnimationFilmHolder::AnimationFilmHolder() {}
AnimationFilmHolder::~AnimationFilmHolder() {}