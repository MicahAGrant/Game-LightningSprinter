#include "states.h"

#include <iostream>

#include "action.h"
#include "game_object.h"
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
}

Action* InAir::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
        // obj.fsm->transition(Transition::Jump, world, obj);
    }
    else if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        // obj.fsm->transition(Transition::Jump, world, obj);
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
    return nullptr;
}

void InAir::on_exit(World&, GameObject& obj) {
    obj.color = {255, 0, 0, 255};
}

// Running
void Running::on_enter(World&, GameObject& obj) {
    obj.color = {255, 255, 0, 255};
}

Action* Running::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    else if (action_type == ActionType::Jump && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
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

void Sprint::on_enter(World&, GameObject& obj) {
    elapsed = cooldown;
    obj.color = {128, 0, 128};
}

void Sprint::update(World& world, GameObject& obj, double dt) {
    elapsed -= .1;
    if (elapsed <= 0 && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
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
    return nullptr;
}

void OnLeftWall::on_enter(World&, GameObject& obj) {
    obj.color = {255, 165, 0};
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
        obj.physics.velocity.y *= 0.1;
    }
}
