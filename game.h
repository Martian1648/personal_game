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

enum class GameMode{Playing, Loss, Victory};

class Game {
public:
    Game(std::string title, int width, int height);
    ~Game();
    void handle_event(SDL_Event* event);
    void input();
    void update();
    void render();
    GameMode mode{GameMode::Playing};
private:
    std::unique_ptr<GameObject> player;
    World* world = nullptr;
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
    void update_enemy(GameObject* obj);
    int current_level{0};
    void load_level();

};