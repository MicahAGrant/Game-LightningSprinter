#pragma once

#include <memory>
#include <SDL3/SDL.h>

#include "tilemap.h"
#include "vec.h"
#include "physics.h"
#include "game_object.h"
#include "level.h"
#include "events.h"
#include "projectile.h"

class GameObject;
class Level;
class Audio;

class World {
public:
    World(const Level& level, Audio& audio, GameObject* player, Events events);
    ~World();

    void add_platform(float x, float y, float width, float height);
    bool collides(const Vec<float>& position) const;
    void update(double dt);
    void update_object(GameObject* obj, double dt);
    void move_to(Vec<float>& position, const Vec<int>& size, Vec<float>& velocity);
    void load_level(const Level& level);

    void build_quadtree();

    // items
    std::map<std::string, std::function<GameObject*()>> available_items;

    Tilemap tilemap;
    Physics physics;
    bool end_level{false};

    GameObject* player;
    Audio* audio;
    Events events;

    std::vector<GameObject*> game_objects;

    void touch_tiles(GameObject& obj);

    QuadTree quadtree;
    bool end_game = false;

    std::vector<Projectile*> projectiles;
    std::vector<Background> backgrounds;
};