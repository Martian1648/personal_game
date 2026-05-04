/*
ὄνομα: action
αἰών: 2/13/2026
βούλημα: game
*/

#include "action.h"

#include "projectile.h"
#include "world.h"

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

void ShootFireball::perform(World &world, GameObject &obj) {
    auto fireball = dynamic_cast<Projectile*>(world.available_items["fireball"]());
    const float direction = obj.facing_left ? -1.0f : 1.0f;
    constexpr float fireball_speed = 12.0f;
    const float spawn_offset_x = obj.facing_left ? -static_cast<float>(fireball->size.x) : static_cast<float>(obj.size.x);

    fireball->physics.position = obj.physics.position;
    fireball->physics.position.x += spawn_offset_x;
    fireball->physics.position.y += 0.3;
    fireball->physics.velocity.x = fireball_speed * direction;
    world.projectiles.push_back(fireball);
}
