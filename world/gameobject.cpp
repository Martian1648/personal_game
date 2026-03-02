/*
ὄνομα: player
αἰών: 1/19/2026
βούλημα: game
*/

#include "gameobject.h"

#include "action.h"
#include "physics.h"


GameObject::GameObject(const Vec<float> &position, const Vec<int> &size, World& world,
    FSM* fsm, Color color, Input* input)
    :size{size},fsm{fsm},  input{input}, color{color}{
    physics.acceleration.y = physics.gravity;
    physics.position = position;
}

GameObject::~GameObject() {
    delete fsm;
    delete input;
}





void GameObject::update(World& world, double dt) {
    fsm->current_state->update(world, *this, dt);
}


std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {physics.position, color};
}


