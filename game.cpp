/*
ὄνομα: game
αἰών: 1/28/2026
βούλημα: game
*/

#include "game.h"
#include "input.h"
#include "assetmanager.h"
#include "level.h"

Game::Game(std::string title, int width, int height) :
graphics{title, width, height},camera{graphics, 64}, dt{1.0/60.0}, lag{0.0},
performance_frequency(SDL_GetPerformanceFrequency()),
prev_counter(SDL_GetPerformanceCounter()){
    //load the first level
    Level level("level_1");
    AssetManager::get_level_details(graphics, level);
    world = new World(level);

    player = std::unique_ptr<GameObject>(world->create_player(level));
    AssetManager::get_game_object_details("player", graphics, *player);
    if (level.player_spawn_location.x >= 0 && level.player_spawn_location.y >= 0) {
        player->physics.position = {
            static_cast<float>(level.player_spawn_location.x),
            static_cast<float>(level.player_spawn_location.y)
        };
    }
    player->fsm->current_state->on_enter(*world, *player);
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
        player->input->handle_input(*world, *player);
        player->update(*world, dt);
        world->update(dt);
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
    camera.render(world->tilemap);

    //draw player
    auto [player_position, color] = player->get_sprite();
    camera.render(*player);

    graphics.update();
}

void Game::handle_event(SDL_Event *event) {
    player->input->collect_discrete_event(event);
}



