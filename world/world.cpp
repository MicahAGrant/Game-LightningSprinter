#include <SDL3/SDL_rect.h>
#include <algorithm>

#include "world.h"
#include "game_object.h"
#include "physics.h"
#include "fsm.h"
#include "states.h"
#include "input.h"
#include "keyboard_input.h"

World::World(int width, int height)
    :tilemap{width, height} {

}


void World::add_platform(float x, float y, float width, float height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; ++j) {
            tilemap(x+j, y+i) = Tile::Platform;
        }
    }
}

bool World::collides(const Vec<float>& position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    return tilemap(x, y) == Tile::Platform;
}


GameObject* World::create_player() {
    // Create FSM
    Transitions transitions = {
        {{StateType::Standing, Transition::Jump}, StateType::InAir},
        {{StateType::InAir, Transition::Stop}, StateType::Standing},
        {{StateType::Standing, Transition::Move}, StateType::Running},
        {{StateType::Running, Transition::Stop}, StateType::Standing},
        {{StateType::Running, Transition::Jump}, StateType::InAir},
        {{StateType::Standing, Transition::BoostLeft}, StateType::Sprint},
        {{StateType::Running, Transition::BoostLeft}, StateType::Sprint},
        {{StateType::Standing, Transition::BoostRight}, StateType::Sprint},
        {{StateType::Running, Transition::BoostRight}, StateType::Sprint},
        {{StateType::Sprint, Transition::Stop}, StateType::Standing},
        {{StateType::Sprint, Transition::Move}, StateType::Running},
        {{StateType::Sprint, Transition::Jump}, StateType::InAir},
        {{StateType::InAir, Transition::BoostLeft}, StateType::Sprint},
        {{StateType::InAir, Transition::BoostRight}, StateType::Sprint},
        {{StateType::Running, Transition::Move}, StateType::OnLeftWall},
        {{StateType::Running, Transition::Move}, StateType::OnRightWall},
        {{StateType::OnLeftWall, Transition::WallJumpLeft}, StateType::InAir},
        {{StateType::OnRightWall, Transition::WallJumpRight}, StateType::InAir}
    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::InAir, new InAir()},
        {StateType::Running, new Running()},
        {StateType::Sprint, new Sprint()},
        {StateType::OnLeftWall, new OnLeftWall()},
        {StateType::OnRightWall, new OnRightWall()}
    };
    FSM* fsm = new FSM{transitions, states, StateType::Standing};

    // player input
    Input* input = new KeyboardInput();

    player = std::make_unique<GameObject>(Vec<float>{10, 5}, Vec<int>{1,1}, *this, fsm, input, Color{255, 0, 0, 255});
    return player.get();
}

void World::update(float dt) {
    // currently only updating player
    auto position = player->physics.position;
    auto velocity = player->physics.velocity;
    auto acceleration = player->physics.acceleration;

    velocity += 0.5f * acceleration * dt;
    position += velocity * dt;
    velocity += 0.5f * acceleration * dt;
    velocity.x *= player->physics.damping;

    velocity.x = std::clamp(velocity.x, -player->physics.terminal_velocity, player->physics.terminal_velocity);
    velocity.y = std::clamp(velocity.y, -player->physics.terminal_velocity, player->physics.terminal_velocity);

    // check for x collisions
    // Check for collisions with the world - x direction
    Vec<float> future_position{position.x, player->physics.position.y};
    Vec<float> future_velocity{velocity.x, 0};
    move_to(future_position, player->size, future_velocity);

    // y direction attempt after (maybe) moving in x
    future_velocity.y = velocity.y;
    future_position.y = position.y;
    move_to(future_position, player->size, future_velocity);

    // update player
    player->physics.position = future_position;
    player->physics.velocity = future_velocity;
}

void World::move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity) {
    bool bottom_right = collides({position.x + size.x, position.y});
    bool bottom_left = collides(position);
    bool top_right = collides({position.x + size.x, position.y + size.y});
    bool top_left = collides({position.x, position.y + size.y});

    // bottom side
    if (bottom_left && bottom_right) {
        position.y = std::ceil(position.y);
        velocity.y = 0;
    }

    // top side
    else if (top_right && top_left) {
        position.y = std::floor(position.y);
        velocity.y = 0;
    }

    // left and right sides
    if (bottom_left && top_left) {
        position.x = std::ceil(position.x);
        velocity.x = 0;
    }

    else if (top_right && bottom_right) {
        position.x = std::floor(position.x);
        velocity.x = 0;
    }

    // float dx_right = std::ceil(position.x) - position.x;
    // float dx_left = position.x - std::floor(position.x);
    // float dy_left = std::ceil
    float dy_tr = (position.y + size.y) - (std::floor(position.y + size.y));
    float dx_tr = std::ceil(position.x)-(position.x+size.x);

    if (collides(position)) {
        float dx = std::ceil(position.x) - position.x;
        float dy = std::ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        }
        else {
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x, position.y + size.y})) {
        float dx = std::ceil(position.x) - position.x;
        float dy = position.y - std::floor(position.y);
        if (dx > dy) {
            position.y = std::floor(position.y);
            velocity.y = 0;
        }
        else {
            position.x = std::ceil(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x + size.x, position.y})) {
        float dx = position.x - std::floor(position.x);
        float dy = std::ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = std::ceil(position.y);
            velocity.y = 0;
        }
        else {
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
    }
    else if (collides({position.x + size.x, position.y + size.y})) {
        float dx = position.x - std::floor(position.x);
        float dy = position.y - std::floor(position.y);
        if (dx > dy) {
            position.y = std::floor(position.y);
            velocity.y = 0;
        }
        else {
            position.x = std::floor(position.x);
            velocity.x = 0;
        }
    }
}