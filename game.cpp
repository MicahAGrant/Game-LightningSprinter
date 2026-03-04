#include "game.h"

#include "asset_manager.h"
#include "game_object.h"

Game::Game(std::string title, int width, int height)
    : graphics{title, width, height}, world{31, 31}, camera{graphics, 64}, dt{1.0/60.0}, lag{0.0}, performance_frequency{SDL_GetPerformanceFrequency()}, prev_counter{SDL_GetPerformanceCounter()} {

    // load the first "level"
    // boundry walls
    world.add_platform(0, 0, 30, 1);
    world.add_platform(0, 0, 1, 30);
    world.add_platform(30, 0, 1, 30);
    world.add_platform(0, 30, 31, 1);

    // platforms
    world.add_platform(3, 7, 4, 1);
    world.add_platform(13, 4, 6, 1);

    // walls
    world.add_platform(10, 10, 1, 7);
    world.add_platform(17, 12, 1, 7);
    world.add_platform(24, 14, 1, 7);

    player = world.create_player();
    // player->sprite = AssetManager::get_game_object_sprite("player", graphics);
    camera.set_location(player->physics.position);
}

void Game::handle_event(SDL_Event* event) {
    player->input->collect_discrete_event(event);
}

void Game::input() {
    player->input->get_input();
    camera.handle_input();
}

void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (float)performance_frequency;
    prev_counter = now;
    while (lag >= dt) {
        player->input->handle_input(world, *player);
        player->update(world, dt);
        world.update(dt);
        // put the camera slightly ahead of the player
        float L = length(player->physics.velocity);
        Vec displacement = 8.0f * player->physics.velocity / (1.0f + L);
        camera.update(player->physics.position + displacement, dt);
        lag -= dt;
    }
}

void Game::render() {
    // clear
    graphics.clear();

    // draw the world
    camera.render(world.tilemap);

    // draw the player
    camera.render(*player);

    // update
    graphics.update();
}
