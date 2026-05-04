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
    fireball->physics.position = (obj.physics.position);
    world.projectiles.push_back(fireball);
    // make the fireball shooting sound
    world.audio->play_sounds("fireball");
}

void AttackProjectile::perform(World& world, GameObject& obj) {
    auto void_attack = dynamic_cast<Projectile*>(world.available_items["void_attack"]());
    void_attack->physics.position = {obj.physics.position.x, obj.physics.position.y};
    world.projectiles.push_back(void_attack);

    auto void_attack2 = dynamic_cast<Projectile*>(world.available_items["void_attack2"]());
    void_attack2->physics.position = {obj.physics.position.x, obj.physics.position.y};
    world.projectiles.push_back(void_attack2);
    // sound effect here
}

void AttackMeleeRight::perform(World& world, GameObject& obj) {
    auto melee_attack = dynamic_cast<Projectile*>(world.available_items["melee_attack"]());
    obj.physics.acceleration.x = obj.physics.walk_acceleration;
    melee_attack->physics.position = obj.physics.position;
    world.projectiles.push_back(melee_attack);
    // sound effect here
}

void AttackMeleeLeft::perform(World& world, GameObject& obj) {
    auto melee_attack = dynamic_cast<Projectile*>(world.available_items["melee_attack2"]());
    obj.physics.acceleration.x = -obj.physics.walk_acceleration;
    melee_attack->physics.position = {obj.physics.position.x, obj.physics.position.y};
    world.projectiles.push_back(melee_attack);
    // sound effect here
}