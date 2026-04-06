/*
ὄνομα: game
αἰών: 1/28/2026
βούλημα: game
*/
#pragma once
#include "audio.h"
#include "camera.h"
#include "events.h"
#include "world/gameobject.h"
#include "world.h"


class Game {
public:
    Game(std::string title, int width, int height);
    ~Game();
    void handle_event(SDL_Event* event);
    void input();
    void update();
    void render();
private:
    std::unique_ptr<GameObject> player;
    World* world;
    Graphics graphics;
    Camera camera;
    Audio audio;
    //timing
    float dt;
    Uint64 performance_frequency;
    Uint64 prev_counter;
    float lag;
    void get_events();
    Events events;
    //level help
    void create_player();
    int current_level{1};
    void load_level();
};