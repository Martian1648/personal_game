/*
ὄνομα: action
αἰών: 2/13/2026
βούλημα: game
*/

#include "action.h"

void Jump::perform(World &, GameObject &obj) {
    obj.physics.velocity.y = obj.physics.jump_velocity;
}

void MoveRight::perform(World &world, GameObject &obj) {
    obj.physics.acceleration.x = obj.physics.walk_acceleration;

}

void MoveLeft::perform(World &world, GameObject &obj) {
    obj.physics.acceleration.x = -obj.physics.walk_acceleration;

}

void SprintLeft::perform(World &world, GameObject &obj) {
    obj.physics.acceleration.x *= obj.physics.sprint_acceleration_multiplier;
}

void SprintRight::perform(World &world, GameObject &obj) {
    obj.physics.acceleration.x *= obj.physics.sprint_acceleration_multiplier;
}

