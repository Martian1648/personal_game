/*
ὄνομα: game
αἰών: 1/28/2026
βούλημα: game
*/
#pragma once
#include "camera.h"
#include "world/gameobject.h"
#include "world.h"


class Game {
public:
    Game(std::string title, int width, int height);
    void handle_event(SDL_Event* event);
    void input();
    void update();
    void render();
private:
    GameObject* player;
    World world;
    Graphics graphics;
    Camera camera;

    //timing
    float dt;
    Uint64 performance_frequency;
    Uint64 prev_counter;
    float lag;
};