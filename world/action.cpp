#include "action.h"
#include "game_object.h"
#include "world.h"

void Jump::perform(World& world, GameObject& obj) {
    obj.physics.velocity.y = obj.physics.jump_velocity;
    world.audio->play_sounds("jump");
}

void MoveRight::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = obj.physics.walk_acceleration;
}

void MoveLeft::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = -obj.physics.walk_acceleration;
}

void BoostRight::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = 4*obj.physics.walk_acceleration;
}

void BoostLeft::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = -4*obj.physics.walk_acceleration;
}
void WallJumpLeft::perform(World&, GameObject& obj) {
    obj.physics.velocity.y = obj.physics.jump_velocity;
    obj.physics.velocity.x += 20;
}

void WallJumpRight::perform(World&, GameObject& obj) {
    obj.physics.velocity.y = obj.physics.jump_velocity;
    obj.physics.velocity.x += -20;
}

void ShootFireball::perform(World &world, GameObject &obj) {
    auto fireball = dynamic_cast<Projectile*>(world.available_items["fireball"]());
    fireball->physics.position = obj.physics.position;
    world.projectiles.push_back(fireball);
    // make the fireball shooting sound
    world.audio->play_sounds("fireball");
}
