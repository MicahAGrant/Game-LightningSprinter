#include "game_object.h"
#include "physics.h"
#include "world.h"

GameObject::GameObject(const Vec<float>& position, const Vec<float>& size, World& world)
    : position{position}, size{size} {
    acceleration = physics.gravity;
}

GameObject::~GameObject() {};

void GameObject::handle_input(World& world) {
    const bool *key_states = SDL_GetKeyboardState(NULL);
    acceleration.x = 0;
    acceleration.y = physics.gravity;
    // velocity.x = velocity.y = 0;

    if (key_states[SDL_SCANCODE_A] || key_states[SDL_SCANCODE_LEFT]) {
        // velocity.x += -8;
        acceleration.x += -physics.walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_D] || key_states[SDL_SCANCODE_RIGHT]) {
        // velocity.x += 8;
        acceleration.x += physics.walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_SPACE]) {
        velocity.y = physics.jump_velocity;
    }
}

void GameObject::update(World& world, double dt) {}

std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {position, {255, 0, 255, 255}};
}