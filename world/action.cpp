#include "action.h"
#include "game_object.h"
#include "world.h"

void Jump::perform(World&, GameObject& obj) {
    obj.physics.velocity.y = obj.physics.jump_velocity;
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