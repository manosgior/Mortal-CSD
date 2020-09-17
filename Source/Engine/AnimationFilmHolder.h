#pragma once

#include <allegro5/allegro.h>
#include <map>
#include "AnimationFilm.h"
#include "BitmapLoader.h"
#include "GTypes.h"
#include <tuple>

class AnimationFilmHolder {
private:
	typedef std::map<std::string, AnimationFilm *> Films;
	Films films;
	BitmapLoader bitmapLoader;
	static AnimationFilmHolder holder;
	AnimationFilmHolder();
	~AnimationFilmHolder();
public:
	static AnimationFilmHolder& Get();	
	void Load(std::vector<std::string> ids, std::vector<std::string> paths, std::vector<std::vector<Rect>> rects);
	void CleanUp();
	const AnimationFilm* GetFilm(const std::string id) const;
};

