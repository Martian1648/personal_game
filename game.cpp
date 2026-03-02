/*
ὄνομα: game
αἰών: 1/28/2026
βούλημα: game
*/

#include "game.h"
#include "input.h"
#include "assetmanager.h"

Game::Game(std::string title, int width, int height) :
graphics{title, width, height},world{32,11},camera{graphics, 64}, dt{1.0/60.0}, lag{0.0},
performance_frequency(SDL_GetPerformanceFrequency()),
prev_counter(SDL_GetPerformanceCounter()){
    //load the first level
    //boundaries
    world.add_platform(0,0,30,1);
    world.add_platform(0,0,1,10);
    world.add_platform(30,0,1,10);
    world.add_platform(0,10,31,1);

    //platforms
    world.add_platform(3,7,4,1);
    world.add_platform(13,4,6,1);

    player = world.create_player();
    player->sprite=AssetManager::get_game_object_sprite("player", graphics);
    camera.set_location(player->physics.position);
}

void Game::input() {
    player->input->get_input();
    camera.handle_input();
}

void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (double) performance_frequency;
    prev_counter = now;
    while (lag >= dt) {
        player->input->handle_input(world, *player);
        player->update(world, dt);
        world.update(dt);
        // put the camera slightly ahead of the player
        float L = length(player->physics.velocity);
        Vec displacement = 8.0f * player->physics.velocity / (1.0f + L);
        camera.update(player->physics.position, dt);
        lag -= dt;
    }
}



void Game::render() {
    // clear
    graphics.clear();

    //draw the world
    camera.render(world.tilemap);

    //draw player
    auto [player_position, color] = player->get_sprite();
    camera.render(*player);

    graphics.update();
}

void Game::handle_event(SDL_Event *event) {
    player->input->collect_discrete_event(event);
}



