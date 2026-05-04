#include "projectile.h"

Projectile::Projectile(std::string name, FSM* fsm, Input* input, double lifetime)
    : GameObject{name, fsm, input, color}, lifetime{lifetime}, direction{direction} {}

void Projectile::update(World& world, double dt) {
    GameObject::update(world, dt);
    elapsed += dt;
}

