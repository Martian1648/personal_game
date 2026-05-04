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
#include "action.h"
#include "aiinput.h"
#include "compositeinput.h"
#include "projectile.h"

Game::Game(std::string title, int width, int height) :
graphics{title, width, height},camera{graphics, 64}, dt{1.0/60.0}, lag{0.0},
performance_frequency(SDL_GetPerformanceFrequency()),
prev_counter(SDL_GetPerformanceCounter()){
    get_events();
    //load the first level
    create_player();
    AssetManager::get_game_object_details("player", graphics, *player);

    load_level();
}

Game::~Game() {
    delete world;
    for (auto [_, event]: events) {
        delete event;
    }
}
void Game::input() {
    switch (mode) {
        case GameMode::Playing:
            player->input->get_input();
            camera.handle_input();
            break;
        case GameMode::Victory:
        case GameMode::Loss:
            break;
    }
}

void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (double) performance_frequency;
    prev_counter = now;
    while (lag >= dt) {
        switch (mode){
            case GameMode::Playing: {
                for (auto obj :world->game_objects) {
                    obj->input->handle_input(*world, *obj);
                }
                world->update(dt);
                // put the camera slightly ahead of the player
                float L = length(player->physics.velocity);
                Vec displacement = 8.0f * player->physics.velocity / (1.0f + L);
                camera.update(player->physics.position, dt);

                if (world->end_level) {
                    load_level();
                }
                if (world->end_game) {
                    if (player->health) {
                        mode = GameMode::Victory;
                    }
                    else {
                        mode = GameMode::Loss;
                    }
                }
                break;
            }
            case GameMode::Victory:
            case GameMode::Loss: {
                audio.stop_background();
                break;
            }

        }
        lag -= dt;
    }
}

void Game::update_enemy(GameObject *obj) {
    Transitions transitions;
    States states;
    if (obj->obj_name == "rat" || obj->obj_name=="burd") {
        transitions = {
            {{StateType::Standing, Transition::Move}, StateType::Patrolling},
            {{StateType::Patrolling, Transition::Stop}, StateType::Standing}
        };
        states = {
            {StateType::Standing, new Standing()},
            {StateType::Patrolling, new Patroling()}
        };

        FSM* fsm = new FSM{transitions, states, StateType::Patrolling};
        obj->fsm = fsm;
        Input* input = new AIinput{};
        input ->next_action_type=ActionType::MoveRight;
        obj->input = input;
    }
    else {
        //throw error or smth idk
    }
}

void Game::render() {
    // clear
    graphics.clear();
    camera.render(world->backgrounds);
    //draw the world
    camera.render(world->tilemap);

    for ( auto& object : world->game_objects) {
        camera.render(*object);
    }

    //draw player
    auto [player_position, color] = player->get_sprite();
    camera.render(*player);

    for (auto& projectile : world->projectiles) {
        camera.render(*projectile);
    }

    if (mode == GameMode::Loss) {
        camera.render_loss();
    }
    if (mode == GameMode::Victory) {
        camera.render_victory();
    }

    graphics.update();

}

void Game::handle_event(SDL_Event *event) {
    switch (mode) {
        case GameMode::Playing: {
            auto action = player->input->collect_discrete_event(event);
            if (action) {
                action->perform(*world, *player);
                delete action;
            }
            break;
        }
        case GameMode::Victory:
        case GameMode::Loss:
            break;

    }
}

void Game::load_level() {
    std::string level_name = "level_" + std::to_string(++current_level);
    Level level{level_name};
    AssetManager::get_level_details(graphics, level);

    //create the world
    delete world;
    world = new World(level, audio, player.get(), events);
    AssetManager::get_available_items("items", graphics, *world);

    //assets for objs
    for ( auto& object : world->game_objects) {
        if (object == world->player)continue;
        update_enemy(object);
        AssetManager::get_game_object_details(object->obj_name+"-enemy", graphics, *object);
    }

    player->physics.position = {static_cast<float>(level.player_spawn_location.x),static_cast<float>(level.player_spawn_location.y)};
    player->fsm->current_state->on_enter(*world, *player);
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
        {{StateType::Standing, Transition::Attack}, {StateType::Attacking}},
        {{StateType::Running, Transition::Attack}, {StateType::Attacking}},
        {{StateType::Attacking, Transition::Chill}, {StateType::Standing}}

    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::InAir, new InAir()},
        {StateType::Running, new Running()},
        {StateType::Sprinting, new Sprinting()},
        {StateType::Attacking, new Attacking()}
    };
    FSM* fsm = new FSM{transitions, states, StateType::Standing};

    Input* input = new CompositeInput();
    player = std::make_unique<GameObject>( "playa", fsm,
        Color{255,0,0,255}, input);
}



void Game::get_events() {
    events["next_level"] = new NextLevel();
    events["end_game"] = new EndGame();
}
