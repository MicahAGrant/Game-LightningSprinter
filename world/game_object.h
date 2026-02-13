#pragma once

#include <utility>

#include "vec.h"
#include "graphics.h"
#include "physics.h"

class World;

class GameObject {
public:
    GameObject(const Vec<float>& position, const Vec<float>& size, World& world);
    ~GameObject();
    void handle_input(World& world);
    void update(World& world, double dt);
    std::pair<Vec<float>, Color> get_sprite() const;

    // Player data
    Vec<float> position;
    Vec<float> size;
    Vec<float> velocity, acceleration;

    Physics physics;
};