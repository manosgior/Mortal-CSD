#include "Audio.h"
#include <fstream>
#include "../Utility/json-develop/include/nlohmann/json.hpp"

using json = nlohmann::json;

AudioHolder AudioHolder::audioHolder = AudioHolder();
std::map<std::string, std::pair<ALLEGRO_SAMPLE *, ALLEGRO_SAMPLE_ID>> AudioHolder::sounds;


void AudioHolder::loadAudio(std::string filepath){
	std::ifstream inputFile;
	inputFile.open(filepath, std::ifstream::in);
	assert(!(inputFile.fail()));
	json audio;

	while (!inputFile.eof()) {
		inputFile >> audio;
		inputFile.peek();
		sounds[audio["action"].get<std::string>()].first = al_load_sample(audio["filepath"].get<std::string>().c_str());
	}

	al_reserve_samples(sounds.size());
}

void AudioHolder::playSound(std::string action){
	al_play_sample(sounds[action].first, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &sounds[action].second);
}

void AudioHolder::playRepeatedSound(std::string action) {
	al_play_sample(sounds[action].first, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &sounds[action].second);
}

void AudioHolder::stopPlayingSound(std::string action) {
	al_stop_sample(&sounds[action].second);
}
