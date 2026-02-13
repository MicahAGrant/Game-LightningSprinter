#pragma once
#include "tilemap.h"
#include "toggle.h"
#include "vec.h"
#include "physics.h"

class Graphics;
class Color;
class Physics;

class Camera {
public:
    Camera(Graphics& graphics, float tilesize);

    void handle_input();
    void update(const Vec<float>& new_location, float dt);
    Vec<float> world_to_screen(const Vec<float>& world_position) const;
    void set_location(const Vec<float>& new_location);

    void render(const Vec<float>& position, const Color& color, bool filled=true) const;
    void render(const Tilemap& tilemap) const;

    Physics physics;

private:
    Graphics &graphics;
    float tilesize;
    Vec<float> location;
    Toggle grid_toggle;

    void calculate_visible_tiles();
    Vec<int> visible_min, visible_max;
    Vec<float> goal, velocity, acceleration;
};
