/*
ὄνομα: projectile
αἰών: 4/27/2026
βούλημα: game
*/

#include "projectile.h"
Projectile::Projectile(std::string name, FSM *fsm, Input *input, double lifetime):
GameObject{name, fsm, Color{0,1,3,129},input},lifetime{lifetime}{

}

void Projectile::update(World &world, double dt) {
    GameObject::update(world, dt);
    elapsed += dt;
}