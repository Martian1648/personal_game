/*
ὄνομα: audio
αἰών: 3/20/2026
βούλημα: game
*/

#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

struct MIX_Mixer;
struct MIX_Audio;
struct MIX_Track;

struct Sound {
    std :: string name;
    std :: string filename;
    bool loop_forever{ false};

};

class Audio {
public:
    Audio();
    ~Audio();

    void load_sounds(const std::vector<Sound>& sounds_to_load);
    void play_sounds(const std::string& sound_name, bool loop_forever_in_background = false,
        float loop_start_time = 0);
    void stop_background();

private:
    SDL_AudioDeviceID device;
    MIX_Mixer* mixer;
    MIX_Track* background_music;
    std::unordered_map<std::string, MIX_Audio*> sounds;
};