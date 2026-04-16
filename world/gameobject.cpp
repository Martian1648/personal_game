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


