#pragma once

#include <memory>
#include <SDL3/SDL.h>

#include "tilemap.h"
#include "vec.h"
#include "physics.h"
#include "game_object.h"

class GameObject;

class World {
public:
    World(int width, int height);
    void add_platform(float x, float y, float width, float height);
    bool collides(const Vec<float>& position) const;
    GameObject* create_player();
    void update(float dt);
    void move_to(Vec<float>& position, const Vec<float>& size, Vec<float>& velocity);

    Tilemap tilemap;
    Physics physics;

private:
    // std::vector<SDL_FRect> platforms;
    std::unique_ptr<GameObject> player;
};