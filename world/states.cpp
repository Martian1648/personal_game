/*
ὄνομα: states
αἰών: 2/16/2026
βούλημα: game
*/

#include "states.h"
#include "action.h"
#include "world.h"


//helper function
bool on_platform(World& world, GameObject& obj) {
    constexpr float epsilon = 1e-4;
    Vec<float> left_foot{obj.physics.position.x +  epsilon, obj.physics.position.y - epsilon};
    Vec<float> right_foot{obj.physics.position.x + obj.size.x-epsilon,obj.physics.position.y-epsilon};
    return world.collides(left_foot) || world.collides(right_foot);
}
void Standing::on_enter(World &, GameObject &obj) {
    obj.color = Color{0,255,255,255};
    obj.physics.acceleration.x = 0;
}

Action *Standing::input(World &world, GameObject &obj, ActionType action_type) {
    if (action_type == ActionType::Jump) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    else if (action_type==ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight();
    }
    else if (action_type==ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft();
    }
    return nullptr;
}

void InAir::on_enter(World &world, GameObject &obj) {
    elapsed = cooldown;
    obj.color = {255,0,0,255};
}

void InAir::update(World &world, GameObject &obj, double dt) {
    elapsed -= dt;
    if (elapsed <= 0 && on_platform(world,obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}

void Running::on_enter(World &, GameObject &obj) {
    obj.color = {255,255,0, 255};
    obj.physics.acceleration.x /= obj.physics.sprint_acceleration_multiplier;
}

Action *Running::input(World &world, GameObject &obj, ActionType action_type) {
     if (action_type == ActionType::None) {
         obj.fsm->transition(Transition::Stop, world, obj);
     }
    else if (action_type == ActionType::Jump) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    else if (action_type == ActionType::SprintLeft) {
        obj.fsm->transition(Transition::Plant, world, obj);
        return new SprintLeft();
    }
    else if (action_type == ActionType::SprintRight) {
        obj.fsm->transition(Transition::Plant, world, obj);
        return new SprintRight();
    }
    return nullptr;
}


void Sprinting::on_enter(World &, GameObject &obj) {
    elapsed= cooldown;
    obj.color={0,255,0,255};
}

void Sprinting::update(World &world, GameObject &obj, double dt) {
    elapsed -= dt;

    if (elapsed <= 0) {
        obj.fsm->transition(Transition::Chill, world, obj);
    }
}




