#pragma once

#include "world.h"
#include "camera.h"
#include "graphics.h"
#include "audio.h"
#include "events.h"

enum class GameMode{Playing, GameOver, Win};

class Game {
public:
    Game(std::string title, int width, int height);
    ~Game();
    void handle_event(SDL_Event* event);
    void input();
    void update();
    void render();

private:
    std::unique_ptr<GameObject> player;
    World* world = nullptr;
    Graphics graphics;

    // timing
    float dt;
    Uint64 performance_frequency;
    Uint64 prev_counter;
    float lag;
    Camera camera;
    Audio audio;
    // SDL counts ticks, pf shows ticks, ticks used to see how much time as past
    // lag keeps track of that and allows for updating at 64fps

    // events
    Events events;
    void get_events();

    // level help
    int current_level{0};
    void load_level();

    void create_player();
    void update_enemy(GameObject& obj, float dt);

    //
    GameMode mode{GameMode::Playing};
};