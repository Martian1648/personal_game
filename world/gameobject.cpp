/*
ὄνομα: player
αἰών: 1/19/2026
βούλημα: game
*/

#include "gameobject.h"
#include "quadtree.h"
#include "action.h"
#include "input.h"
#include "physics.h"
#include "../game.h"


GameObject::GameObject(std::string name,
                       FSM* fsm, Color color, Input* input)
    :obj_name{name},fsm{fsm},  input{input}, color{color}{

}

GameObject::~GameObject() {
    delete fsm;
    delete input;
}

AABB GameObject::get_bounding_box() {
    Vec<float> half_size = {size.x / 2.0f, size.y/2.0f};
    Vec<float>center = {
        physics.position.x + half_size.x,
        physics.position.y + half_size.y
    };
    AABB bounding_box {center, half_size};
    return bounding_box;
}


void GameObject::update(World& world, double dt) {
    if (fsm != nullptr) {
        fsm->current_state->update(world, *this, dt);
    }
    sprites[sprite_name].update(dt);
    set_sprite(sprite_name);
    if (i_frames_remaining > 0.0) i_frames_remaining -=dt;
}


std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {physics.position, color};
}


void GameObject::set_sprite(const std::string &next_sprite) {
    if (next_sprite != sprite_name) {
        sprites[sprite_name].reset();

        auto itr=sprites.find(next_sprite);
        if (itr != sprites.end()) {
            sprite_name = next_sprite;

        }
        else {
            sprite_name = "idle"; //assume
        }
    }

    sprite=sprites[sprite_name].get_sprite();
}


void GameObject::take_damage(int attack_damage) {
    if (i_frames_remaining > 0.0) return;
    health -= attack_damage;
    i_frames_remaining = 2;
    if (health <= 0) {
        is_alive = false;
    }
}

bool GameObject::flash_sprite() const {
    if (i_frames_remaining <= 0.0) {
        return false;
    }

    // alternate overlay on/off every 80 ms
    return ((SDL_GetTicks() / 80) % 2) == 0;
}