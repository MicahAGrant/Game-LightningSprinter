#pragma once

#include <memory>
#include <SDL3/SDL.h>

#include "tilemap.h"
#include "vec.h"
#include "physics.h"
#include "game_object.h"
#include "level.h"

class World {
public:
    World(const Level& level);
    void add_platform(float x, float y, float width, float height);
    bool collides(const Vec<float>& position) const;
    GameObject* create_player(const Level& level);
    void update(float dt);
    void move_to(Vec<float>& position, const Vec<float>& size, Vec<float>& velocity);
    void load_level(const Level& level);

    Tilemap tilemap;
    Physics physics;

private:
    // std::vector<SDL_FRect> platforms;
    GameObject* player;
};