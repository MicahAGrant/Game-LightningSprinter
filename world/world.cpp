#include <SDL3/SDL_rect.h>
#include <algorithm>

#include "world.h"
#include "game_object.h"
#include "physics.h"

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
    player = std::make_unique<GameObject>(Vec<float>{10, 5}, Vec<float>{1.0f, 1.0f}, *this);
    return player.get();
}

void World::update(float dt) {
    // currently only updating player
    auto position = player->position;
    auto velocity = player->velocity;
    auto acceleration = player->acceleration;

    velocity += 0.5f * acceleration * dt;
    position += velocity * dt;
    velocity += 0.5f * acceleration * dt;
    velocity.x *= physics.damping;

    velocity.x = std::clamp(velocity.x, -physics.terminal_velocity, physics.terminal_velocity);
    velocity.y = std::clamp(velocity.y, -physics.terminal_velocity, physics.terminal_velocity);

    // check for collisions in the world - x direction
    Vec<float> future_position{position.x, player->position.y};
    Vec<float> future_veloctiy{velocity.x, 0};
    move_to(future_position, player->size, future_veloctiy);

    player->position.x = future_position.x;
    player->velocity.x = future_veloctiy.x;

    // now y direction after (maybe) moving in x
    future_veloctiy.y = velocity.y;
    future_position.y = position.y;
    move_to(future_position, player->size, future_veloctiy);

    // update the player position and velocity
    player->position = future_position;
    player->velocity = future_veloctiy;
    player->acceleration = physics.gravity;
}

void World::move_to(Vec<float>& position, const Vec<float>& size, Vec<float>& velocity) {
    // test for collisions on the bottom or top first
    // top
    // if (velocity.y > 0) {
    //     Vec<float> top_left_corner {position.x, position.y+size.y};
    //     Vec<float> top_right_corner {position.x + size.x, position.y + size.y};
    //     if (collides(top_left_corner) || collides(top_right_corner)) {
    //         position.y = std::floor(position.y + size.y) - size.y;
    //         velocity.y = 0;
    //     }
    // }

    // test sides first. if both collide move backward
    // bottom side
    if (collides(position) && collides({position.x + size.x, position.y})) {
        position.y = std::ceil(position.y);
        velocity.y = 0;
    }

    // top side
    else if (collides({position.x + size.x, position.y + size.y}) && collides({position.x, position.y + size.y})) {
        position.y = std::floor(position.y);
        velocity.y = 0;
    }

    // left and right sides
    if (collides(position) && collides({position.x, position.y + size.y})) {
        position.x = std::ceil(position.x);
        velocity.x = 0;
    }

    else if (collides({position.x + size.x, position.y + size.y}) && collides({position.x + size.x, position.y})) {
        position.x = std::floor(position.x);
        velocity.x = 0;
    }
}