/*
ὄνομα: world
αἰών: 1/19/2026
βούλημα: game
*/

#include "world.h"
#include <SDL3/SDL_rect.h>
#include <algorithm>
#include "audio.h"
#include "events.h"
#include "physics.h"
#include "gameobject.h"
#include "states.h"
#include "keyboardinput.h"

World::World(const Level& level, Audio& audio,GameObject* player,std::map<std::string, Event*>& events):
tilemap{level.width,level.height}, audio{&audio}, player{player}, events{events}{
    load_level(level);
}


void World::add_platform(float x, float y, float width, float height) {
    SDL_FRect rect {x,y,width,height};
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tilemap(x+j, y+i) = Tile{};
        }
    }
}



bool World::collides(const Vec<float> &position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    if (x >= tilemap.width || x < 0 || y >= tilemap.height || y < 0) {
        return true;
    }
    return tilemap(x,y).blocking;
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

    touch_tiles(*player);
}

void World::move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity) {
    constexpr float epsilon = 1e-4f;
    const Vec<float> bottom_left{position.x + epsilon, position.y + epsilon};
    const Vec<float> bottom_right{position.x + size.x - epsilon, position.y + epsilon};
    const Vec<float> top_left{position.x + epsilon, position.y + size.y - epsilon};
    const Vec<float> top_right{position.x + size.x - epsilon, position.y + size.y - epsilon};

    // test sides first. if both collide move backward
    // bottom side
    if (collides(bottom_left) && collides(bottom_right)) {
        position.y = std::ceil(position.y);
        velocity.y = 0;
    }
    // top side
    else if (collides(top_left) && collides(top_right)) {
        position.y = std::floor(position.y);
        velocity.y = 0;
    }
    // left side
    if (collides(bottom_left) && collides(top_left)) {
        position.x = std::ceil(position.x);
        velocity.x = 0;
    }
    // right side
    else if (collides(bottom_right) && collides(top_right)) {
        position.x = std::floor(position.x);
        velocity.x = 0;
    }
    // test corners next, move back in smaller axis
    if (collides(bottom_left)) {
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
    else if (collides(top_left)) {
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
    else if (collides(bottom_right)) {
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
    else if (collides(top_right)) {
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

void World::load_level(const Level &level) {
    for (const auto& [pos, tile_id] : level.tile_locations) {
        tilemap(pos.x, pos.y) = level.tile_types.at(tile_id);
    }
    audio->load_sounds({});

    for (const auto& [pos, enemy_name] : level.enemy_locations) {
        GameObject enemy{enemy_name, nullptr,{12,12,12,12}, nullptr};
        enemy.physics.position = pos;
        game_objects.push_back(enemy);
    }
}

void World::touch_tiles(GameObject &obj) {
    int x = std::floor(obj.physics.position.x);
    int y = std::floor(obj.physics.position.y);
    const std::vector<Vec<int>> displacements{{0,0},{obj.size.x, 0}, {0, obj.size.y}, {obj.size.x, obj.size.y}};
    for (const auto& displacement:displacements) {
        Tile& tile = tilemap(x+displacement.x, y+displacement.y);
        if (!tile.event_name.empty()) {
            auto itr = events.find(tile.event_name);
            if (itr==events.end()) {
                throw std::runtime_error("Cannot find event: " + tile.event_name);
            }
            itr->second->perform(*this, obj);
        }
    }
}

