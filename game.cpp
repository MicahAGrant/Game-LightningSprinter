#include "game.h"

Game::Game(std::string title, int width, int height)
    : graphics{title, width, height}, world{31, 11}, dt{1.0/60.0}, lag{0.0},
      performance_frequency{SDL_GetPerformanceFrequency()},
      prev_counter{SDL_GetPerformanceCounter()},
      camera{graphics, 64} {

    // load the first "level"
    // boundry walls
    world.add_platform(0, 0, 30, 1);
    world.add_platform(0, 0, 1, 10);
    world.add_platform(30, 0, 1, 10);
    world.add_platform(0, 10, 31, 1);

    // platforms
    world.add_platform(3, 7, 4, 1);
    world.add_platform(13, 4, 6, 1);

    player = world.create_player();
    camera.set_location(player->position);
}


void Game::input() {
    player->handle_input(world);
    camera.handle_input();
}

void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (float)performance_frequency;
    prev_counter = now;
    while (lag >= dt) {
        world.update(dt);
        // put the camera slightly ahead of the player
        float L = length(player->velocity);
        Vec displacement = 8.0f * player->velocity / (1.0f + L);
        camera.update(player->position + displacement, dt);
        lag -= dt;
    }
}

void Game::render() {
    // clear
    graphics.clear();

    // draw the world
    camera.render(world.tilemap);

    // draw the player
    auto [player_position, color] = player->get_sprite();
    camera.render(player_position, color);

    // update
    graphics.update();
}

