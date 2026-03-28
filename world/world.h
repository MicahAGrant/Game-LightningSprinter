#pragma once

#include <memory>
#include <SDL3/SDL.h>

#include "tilemap.h"
#include "vec.h"
#include "physics.h"
#include "game_object.h"
#include "level.h"
#include "events.h"

class GameObject;
class Level;
class Audio;

class World {
public:
    World(const Level& level, Audio& audio, GameObject* player, Events events);

    void add_platform(float x, float y, float width, float height);
    bool collides(const Vec<float>& position) const;
    GameObject* create_player(const Level& level);
    void update(float dt);
    void move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity);
    void load_level(const Level& level);

    Tilemap tilemap;
    Physics physics;
    bool end_level{false};

private:
    GameObject* player;
    Audio* audio;
    Events events;
};