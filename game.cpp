#include "game.h"

#include <iostream>

#include "ai_input.h"
#include "asset_manager.h"
#include "game_object.h"
#include "fsm.h"
#include "keyboard_input.h"
#include "states.h"

using Events = std::map<std::string, Event*>;

using Events = std::map<std::string, Event*>;

Game::Game(std::string title, int width, int height)
    : graphics{title, width, height}, camera{graphics, 64}, dt{1.0 / 60.0},
      performance_frequency{SDL_GetPerformanceFrequency()}, prev_counter{SDL_GetPerformanceCounter()}, lag{0.0} {
    // load events
    get_events();

    // Give player its assets then put it in the correct state
    create_player();
    AssetManager::get_game_object_details("player", graphics, *player);

    // load first level
    load_level();
}

void Game::handle_event(SDL_Event* event) {
    switch (mode) {
        case GameMode::Playing:
            auto action = player->input->collect_discrete_event(event);
        if (action) {
            action->perform(*world, *player);
            delete action;
        }
        break;
    }
}

void Game::input() {
    switch (mode) {
        case GameMode::Playing:
        player->input->get_input();
        camera.handle_input();
        break;
    }
}

void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (float)performance_frequency;
    prev_counter = now;
    while (lag >= dt) {
        switch (mode) {
            case GameMode::Playing:
                for (auto obj : world->game_objects) {
                    obj->input->handle_input(*world, *obj);
                }

                world->update(dt);
                // put the camera slightly ahead of the player
                float L = length(player->physics.velocity);
                Vec displacement = 8.0f * player->physics.velocity / (1.0f + L);
                camera.update(player->physics.position + displacement, dt);

                // check for level end
                if (world->end_level) {
                    load_level();
                }
                // check for level end
                if (world->end_game) {
                    mode = GameMode::GameOver;
                }
                // do one where game ends and if player alive, then win!
                if (world->end_game && player->is_alive) {
                    mode = GameMode::Win;
                }
                break;
        }
        lag -= dt;
    }

}

void Game::render() {
    // clear
    graphics.clear();

    // draw the backgrounds
    camera.render(world->backgrounds);

    // draw the world
    camera.render(world->tilemap);

    // draw the player
    camera.render(*player);

    // draw enemies
    for (auto& obj : world->game_objects) {
        camera.render(*obj);
    }

    // draw projectiles
    for (auto& projectile : world->projectiles) {
        camera.render(*projectile);
    }

    // game over screen (lose/die)
    if (mode == GameMode::GameOver) {
        camera.render_game_over();
    }

    // win screen
    if (mode == GameMode::Win) {
        camera.render_win();
    }

    // update
    graphics.update();
}

Game::~Game() {
    delete world;
    for (auto [_, event]: events) {
        delete event;
    }
}


void Game::get_events() {
    events["next_level"] = new NextLevel();
    events["win"] = new Win();
}

void Game::load_level() {
    std::string level_name = "level_" + std::to_string(++current_level);
    Level level{level_name};
    AssetManager::get_level_details(graphics, level);

    // create the world
    delete world;
    world = new World(level, audio, player.get(), events);

    // get available items
    AssetManager::get_available_items("items", graphics, *world);

    // assets for objs
    for (auto obj : world->game_objects) {
        if (obj == world->player) continue;
        update_enemy(*obj, dt);
        AssetManager::get_game_object_details(obj->obj_name + "-enemy", graphics, *obj, true);
    }

    player->physics.position = {static_cast<float>(level.player_spawn_location.x), static_cast<float>(level.player_spawn_location.y)};
    player->fsm->current_state->on_enter(*world, *player);
    camera.set_location(player->physics.position);
    audio.play_sounds("background", true);
}

void Game::create_player() {
    Transitions transitions = {
        {{StateType::Standing, Transition::Jump}, StateType::InAir},
        {{StateType::InAir, Transition::Stop}, StateType::Standing},
        {{StateType::Standing, Transition::Move}, StateType::Running},
        {{StateType::Running, Transition::Stop}, StateType::Standing},
        {{StateType::Running, Transition::Jump}, StateType::InAir},
        {{StateType::Standing, Transition::BoostLeft}, StateType::Sprint},
        {{StateType::Running, Transition::BoostLeft}, StateType::Sprint},
        {{StateType::Standing, Transition::BoostRight}, StateType::Sprint},
        {{StateType::Running, Transition::BoostRight}, StateType::Sprint},
        {{StateType::InAir, Transition::Move}, StateType::Running},
        {{StateType::Sprint, Transition::Stop}, StateType::Standing},
        {{StateType::Sprint, Transition::Move}, StateType::Running},
        {{StateType::Sprint, Transition::Jump}, StateType::InAir},
        {{StateType::InAir, Transition::BoostLeft}, StateType::Sprint},
        {{StateType::InAir, Transition::BoostRight}, StateType::Sprint},
        {{StateType::OnLeftWall, Transition::WallJumpLeft}, StateType::InAir},
        {{StateType::OnRightWall, Transition::WallJumpRight}, StateType::InAir},
        {{StateType::Standing, Transition::AttackAll}, StateType::AttackAll},
        {{StateType::AttackAll, Transition::Stop}, StateType::Standing},
        {{StateType::Standing, Transition::MeleeLeft}, StateType::Melee},
        {{StateType::Melee, Transition::Stop}, StateType::Standing},
        {{StateType::Running, Transition::MeleeLeft}, StateType::Melee},
        {{StateType::Melee, Transition::Move}, StateType::Running},
        {{StateType::Standing, Transition::MeleeRight}, StateType::Melee},
        {{StateType::Melee, Transition::Stop}, StateType::Standing},
        {{StateType::Running, Transition::MeleeRight}, StateType::Melee},
        {{StateType::Melee, Transition::Move}, StateType::Running}
    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::InAir, new InAir()},
        {StateType::Running, new Running()},
        {StateType::Sprint, new Sprint()},
        {StateType::OnLeftWall, new OnLeftWall()},
        {StateType::OnRightWall, new OnRightWall()},
        {StateType::AttackAll, new AttackAll()},
        {StateType::Melee, new Melee()}
    };
    FSM* fsm = new FSM{transitions, states, StateType::Standing};

    // player input
    Input* input = new KeyboardInput();

    player = std::make_unique<GameObject>("player", fsm, input, Color{255, 0, 0, 255});
}

void Game::update_enemy(GameObject& obj, float dt) {
    Transitions transitions;
    States states;

    if (obj.obj_name == "tree-monster" || obj.obj_name == "void-monster") {
        transitions = {
            {{StateType::Standing, Transition::Move}, StateType::Patrolling},
            {{StateType::Patrolling, Transition::Stop}, StateType::Standing},
            {{StateType::Standing, Transition::Move}, StateType::Attack}
        };
        states = {
            {StateType::Standing, new Standing()},
            {StateType::Patrolling, new Patrolling()},
            {StateType::Attack, new Attack()}
        };
    }

    else {
        // throw an error?
    }

    FSM* fsm = new FSM{transitions, states, StateType::Patrolling};
    obj.fsm = fsm;

    Input* input = new AiInput{};
    input->next_action_type = ActionType::MoveRight;
    obj.input = input;
}
