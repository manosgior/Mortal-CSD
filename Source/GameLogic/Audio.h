#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <string>
#include <map>

class AudioHolder {
private:
	static std::map<std::string, std::pair<ALLEGRO_SAMPLE *, ALLEGRO_SAMPLE_ID>> sounds;
	static AudioHolder audioHolder;
public:
	static void loadAudio(std::string filepath);
	static void playSound(std::string action);
	static void playRepeatedSound(std::string action);
	static void stopPlayingSound(std::string action);
};
