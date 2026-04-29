#include "states.h"

#include <iostream>

#include "action.h"
#include "game_object.h"
#include "random.h"
#include "world.h"

// Helper
bool on_platform(World& world, GameObject& obj) {
    constexpr float epsilon = 1e-4;
    Vec<float> left_foot{obj.physics.position.x + epsilon, obj.physics.position.y - epsilon};
    Vec<float> right_foot{obj.physics.position.x + obj.size.x - epsilon, obj.physics.position.y - epsilon};
    return world.collides(left_foot) || world.collides(right_foot);
}

bool on_left_wall(World& world, GameObject& obj) {
    constexpr float epsilon = 1e-4;
    Vec<float> upper_left_side{obj.physics.position.x - epsilon, obj.physics.position.y + obj.size.y - epsilon};
    Vec<float> lower_left_side{obj.physics.position.x - epsilon, obj.physics.position.y + epsilon};
    return world.collides(upper_left_side) || world.collides(lower_left_side);
}

bool on_right_wall(World& world, GameObject& obj) {
    constexpr float epsilon = 1e-4;
    Vec<float> upper_right_side{obj.physics.position.x + obj.size.x + epsilon, obj.physics.position.y + epsilon};
    Vec<float> lower_right_side{obj.physics.position.x + obj.size.x + epsilon, obj.physics.position.y + epsilon};
    return world.collides(upper_right_side) || world.collides(lower_right_side);
}

// Standing
void Standing::on_enter(World&, GameObject& obj) {
    obj.color = {255, 0, 0, 255};
    obj.set_sprite("idle");
    obj.physics.acceleration.x = 0;
}

Action* Standing::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::Jump) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    else if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight;
    }
    else if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft;
    }
    else if (action_type == ActionType::BoostLeft) {
        obj.fsm->transition(Transition::BoostLeft, world, obj);
        return new BoostLeft;
    }
    else if (action_type == ActionType::BoostRight) {
        obj.fsm->transition(Transition::BoostRight, world, obj);
        return new BoostRight;
    }
    else if (action_type == ActionType::AttackAll) {
        obj.fsm->transition(Transition::AttackAll, world, obj);
    }

    return nullptr;
}

void Standing::update(World& world, GameObject& obj, double dt) {
    if (!on_platform(world, obj)) {
        obj.fsm->transition(Transition::Jump, world, obj);
    }
}


// In Air
void InAir::on_enter(World& world, GameObject& obj) {
    elapsed = cooldown;
    obj.set_sprite("jumping");
    obj.color = {0, 0, 255, 255};
}

void InAir::update(World& world, GameObject& obj, double dt) {
    elapsed -= dt;
    if (elapsed <= 0 && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    if (!on_platform(world, obj)) {
        obj.fsm->transition(Transition::Jump, world, obj);
    }
    if (on_right_wall(world, obj) || on_left_wall(world, obj)) {
        obj.physics.velocity.y *= 0.8;
    }
}

Action* InAir::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    else if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight;
    }
    else if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft;
    }
    else if (action_type == ActionType::BoostLeft) {
        obj.fsm->transition(Transition::BoostLeft, world, obj);
        return new BoostLeft;
    }
    else if (action_type == ActionType::BoostRight) {
        obj.fsm->transition(Transition::BoostRight, world, obj);
        return new BoostRight;
    }
    else if (action_type == ActionType::Jump && on_left_wall(world, obj)) {
        obj.fsm->transition(Transition::WallJumpLeft, world, obj);
        return new WallJumpLeft;
    }
    else if (action_type == ActionType::Jump && on_right_wall(world, obj)) {
        obj.fsm->transition(Transition::WallJumpRight, world, obj);
        return new WallJumpRight;
    }
    return nullptr;
}

void InAir::on_exit(World&, GameObject& obj) {
    obj.color = {255, 0, 0, 255};
}

// Running
void Running::on_enter(World&, GameObject& obj) {
    obj.color = {255, 255, 0, 255};
    obj.set_sprite("running");
}

Action* Running::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    else if (action_type == ActionType::Jump && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    else if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight();
    }
    else if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft();
    }
    else if (action_type == ActionType::BoostRight) {
        obj.fsm->transition(Transition::BoostRight, world, obj);
        return new BoostRight();
    }
    else if (action_type == ActionType::BoostLeft) {
        obj.fsm->transition(Transition::BoostLeft, world, obj);
        return new BoostLeft();
    }
    else if (action_type == ActionType::Jump && on_left_wall(world, obj)) {
        obj.fsm->transition(Transition::WallJumpLeft, world, obj);
        return new WallJumpLeft;
    }
    else if (action_type == ActionType::Jump && on_right_wall(world, obj)) {
        obj.fsm->transition(Transition::WallJumpRight, world, obj);
        return new WallJumpRight;
    }
    return nullptr;
}

void Running::update(World& world, GameObject& obj, double dt) {
    if (on_right_wall(world, obj) || on_left_wall(world, obj)) {
        obj.set_sprite("sliding");
        obj.physics.velocity.y *= 0.8;
        obj.color = {255, 165, 0};
    }
    else if (!on_platform(world, obj)) {
        obj.set_sprite("jumping");
        obj.color = {0, 0, 255, 255};
    }
    else if (on_platform(world, obj)) {
        obj.set_sprite("running");
        obj.color = {255, 255, 0};
    }
}



void Sprint::on_enter(World&, GameObject& obj) {
    elapsed = cooldown;
    obj.set_sprite("sprinting");
    obj.color = {128, 0, 128};
}

void Sprint::update(World& world, GameObject& obj, double dt) {
    if (on_right_wall(world, obj) || on_left_wall(world, obj)) {
        obj.set_sprite("sliding");
        obj.physics.velocity.y *= 0.8;
        obj.color = {255, 165, 0};
    }
    else if (!on_platform(world, obj)) {
        obj.set_sprite("jumping");
        obj.color = {0, 0, 255, 255};
    }
    else if (on_platform(world, obj)) {
        obj.set_sprite("sprinting");
        obj.color = {128, 0, 128};
    }
}


Action* Sprint::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    else if (action_type == ActionType::Jump && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    else if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight;
    }
    else if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft;
    }
    else if (action_type == ActionType::Jump && on_left_wall(world, obj)) {
        obj.fsm->transition(Transition::WallJumpLeft, world, obj);
        return new WallJumpLeft;
    }
    else if (action_type == ActionType::Jump && on_right_wall(world, obj)) {
        obj.fsm->transition(Transition::WallJumpRight, world, obj);
        return new WallJumpRight;
    }
    else if (action_type == ActionType::BoostRight) {
        obj.fsm->transition(Transition::BoostRight, world, obj);
        return new BoostRight();
    }
    else if (action_type == ActionType::BoostLeft) {
        obj.fsm->transition(Transition::BoostLeft, world, obj);
        return new BoostLeft();
    }
    return nullptr;
}

void OnLeftWall::on_enter(World&, GameObject& obj) {
    obj.color = {255, 165, 0};
    obj.set_sprite("sliding");
}

Action* OnLeftWall::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    else if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft;
    }
    else if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight;
    }
    return nullptr;
}

void OnLeftWall::update(World& world, GameObject& obj, double dt) {
    if (on_left_wall(world, obj)) {
        obj.physics.velocity.y *= 0.1;
    }
}

void OnRightWall::on_enter(World&, GameObject& obj) {
    obj.color = {255, 165, 0};
    obj.set_sprite("sliding");
}

Action* OnRightWall::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    else if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft;
    }
    else if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight;
    }
    return nullptr;
}

void OnRightWall::update(World& world, GameObject& obj, double dt) {
    if (on_right_wall(world, obj)) {
        obj.physics.gravity *= 0.1;
    }
    std::cout << "This works";
}

// AttackAll
void AttackAll::on_enter(World& world, GameObject& obj) {
    obj.color = {255, 100, 0, 255};
    for (auto& enemy : world.game_objects) {
        if (enemy == world.player) continue;
        enemy->take_damage(obj.damage);
    }
    elapsed = 0;
}


void AttackAll::update(World& world, GameObject& obj, double dt) {
    elapsed += dt;
    if (elapsed >= cooldown) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}

// Patrolling
void Patrolling::on_enter(World& world, GameObject& obj) {
    // set cooldown to a random amount of time 3-10 seconds
    elapsed = 0;
    cooldown = randint(3,10);
    Running::on_enter(world, obj);
}


Action* Patrolling::input(World& world, GameObject& obj, ActionType action_type) {
    if (elapsed >= cooldown) {
        return Running::input(world, obj, ActionType::None);
    }
    return Running::input(world, obj, action_type);
}


void Patrolling::update(World&, GameObject&, double dt) {
    elapsed += dt;
}