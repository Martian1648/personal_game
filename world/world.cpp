/*
ὄνομα: world
αἰών: 1/19/2026
βούλημα: game
*/

#include "world.h"
#include <SDL3/SDL_rect.h>
#include <algorithm>

#include "physics.h"
#include "gameobject.h"
#include "states.h"
#include "keyboardinput.h"

World::World(int width, int height):
tilemap{width,height}{

}


void World::add_platform(float x, float y, float width, float height) {
    SDL_FRect rect {x,y,width,height};
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tilemap(x+j, y+i) = Tile::Platform;
        }
    }
}



bool World::collides(const Vec<float> &position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    if (x >= tilemap.width || x < 0 || y >= tilemap.height || y < 0) {
        return true;
    }
    return tilemap(x,y) == Tile::Platform;
}


GameObject *World::create_player() {
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
    player = std::make_unique<GameObject>(Vec<float>{10,3}, Vec<int>{1, 1},*this, fsm,
        Color{255,0,0,255}, input);
    return player.get();
}



void World::update(float dt) {
    // currently only updating player
    auto position = player->physics.position;
    auto velocity = player->physics.velocity;
    auto acceleration = player->physics.acceleration;

    velocity += 0.5f * acceleration * dt;
    position += velocity * dt;
    velocity += 0.5f * acceleration * dt;
    velocity.x *= player->physics.damping;

    velocity.x = std::clamp(velocity.x, -player->physics.terminal_velocity, player->physics.terminal_velocity);
    velocity.y = std::clamp(velocity.y, -player->physics.terminal_velocity, player->physics.terminal_velocity);

    // check for x collisions
    // Check for collisions with the world - x direction
    Vec<float> future_position{position.x, player->physics.position.y};
    Vec<float> future_velocity{velocity.x, 0};
    move_to(future_position, player->size, future_velocity);

    // y direction attempt after (maybe) moving in x
    future_velocity.y = velocity.y;
    future_position.y = position.y;
    move_to(future_position, player->size, future_velocity);

    // update player
    player->physics.position = future_position;
    player->physics.velocity = future_velocity;
}

void World::move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity) {
    // test sides first. if both collide move backward
    // bottom side
    if (collides(position) && collides({position.x + size.x, position.y})) {
        position.y = std::ceil(position.y);
        velocity.y = 0;
    }
    // top side
    else if (collides({position.x, position.y + size.y}) && collides({position.x + size.x, position.y + size.y})) {
        position.y = std::floor(position.y);
        velocity.y = 0;
    }
    // left side
    if (collides(position) && collides({position.x, position.y + size.y})) {
        position.x = std::ceil(position.x);
        velocity.x = 0;
    }
    // right side
    else if (collides({position.x + size.x, position.y}) && collides({position.x + size.x, position.y + size.y})) {
        position.x = std::floor(position.x);
        velocity.x = 0;
    }
    // test corners next, move back in smaller axis
    if (collides(position)) {
        float dx = std::ceil(position.x) - position.x;
        float dy = std::ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        }
        else {
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x, position.y + size.y})) {
        float dx = std::ceil(position.x) - position.x;
        float dy = position.y - std::floor(position.y);
        if (dx > dy) {
            position.y = std::floor(position.y);
            velocity.y = 0;
        }
        else {
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x + size.x, position.y})) {
        float dx = position.x - std::floor(position.x);
        float dy = std::ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        }
        else {
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x + size.x, position.y + size.y})) {
        float dx = position.x - std::floor(position.x);
        float dy = position.y - std::floor(position.y);
        if (dx > dy) {
            position.y = std::floor(position.y);
            velocity.y = 0;
        }
        else {
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
    }
}

