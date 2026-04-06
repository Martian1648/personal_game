/*
ὄνομα: game
αἰών: 1/28/2026
βούλημα: game
*/

#include "game.h"
#include "input.h"
#include "assetmanager.h"
#include "keyboardinput.h"
#include "level.h"
#include "states.h"

Game::Game(std::string title, int width, int height) :
graphics{title, width, height},camera{graphics, 64}, dt{1.0/60.0}, lag{0.0},
performance_frequency(SDL_GetPerformanceFrequency()),
prev_counter(SDL_GetPerformanceCounter()){
    get_events();
    //load the first level
    Level level("level_1");
    create_player();
    AssetManager::get_game_object_details("player", graphics, *player);

    AssetManager::get_level_details(graphics, level);
    world = new World(level, audio, player.get(), events);


    AssetManager::get_game_object_details("player", graphics, *player);
    player->physics.position = {static_cast<float>(level.player_spawn_location.x),static_cast<float>(level.player_spawn_location.y)};

    player->fsm->current_state->on_enter(*world, *player);
    camera.set_location(player->physics.position);
    audio.play_sounds("background", true, 46.28);
}

Game::~Game() {
    delete world;
    for (auto [_, event]: events) {
        delete event;
    }
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
        if (world->end_level) {
            load_level();
        }
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

void Game::load_level() {
    std::string level_name = "level_" + std::to_string(++current_level);
    Level level{level_name};
    AssetManager::get_level_details(graphics, level);

    //create the world
    delete world;
    world = new World(level, audio, player.get(), events);

    //assets for objs
    for ( auto& object : world->game_objects) {
        AssetManager::get_game_object_details(object.obj_name+"-enemy", graphics, object);
    }

    player->physics.position = {static_cast<float>(level.player_spawn_location.x),static_cast<float>(level.player_spawn_location.y)};
    camera.set_location(player->physics.position);
    audio.play_sounds("background", true, 46.28);
}

void Game::create_player() {
    Transitions transitions = {
        {{StateType::Standing, Transition::Jump}, StateType::InAir},
        {{StateType::InAir, Transition::Stop}, StateType::Standing},
        {{StateType::Standing, Transition::Move}, StateType::Running},
        {{StateType::Running, Transition::Stop}, StateType::Standing},
        {{StateType::Running, Transition::Jump}, StateType::InAir},
        {{StateType::Running, Transition::Plant}, {StateType::Sprinting}},
        {{StateType::Sprinting, Transition::Chill}, {StateType::Running}},

    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::InAir, new InAir()},
        {StateType::Running, new Running()},
        {StateType::Sprinting, new Sprinting()},
    };
    FSM* fsm = new FSM{transitions, states, StateType::Standing};

    Keyboard_Input* input = new Keyboard_Input();
    player = std::make_unique<GameObject>( "playa", fsm,
        Color{255,0,0,255}, input);
}



void Game::get_events() {
    events["next_level"] = new NextLevel();
}