#include "keyboard_input.h"
#include "game_object.h"
#include "fsm.h"

void KeyboardInput::get_input() {
    if (next_action_type == ActionType::Jump) return;

    const bool *key_states = SDL_GetKeyboardState(NULL);

    if (key_states[SDL_SCANCODE_LSHIFT]) {
        if (key_states[SDL_SCANCODE_D] || key_states[SDL_SCANCODE_RIGHT]) {
            next_action_type = ActionType::BoostRight;
        }
        else if (key_states[SDL_SCANCODE_A] || key_states[SDL_SCANCODE_LEFT]) {
            next_action_type = ActionType::BoostLeft;
        }
    }

    else if (key_states[SDL_SCANCODE_D] || key_states[SDL_SCANCODE_RIGHT]) {
        next_action_type = ActionType::MoveRight;
    }

    else if (key_states[SDL_SCANCODE_A] || key_states[SDL_SCANCODE_LEFT]) {
        next_action_type = ActionType::MoveLeft;
    }
}

void KeyboardInput::handle_input(World& world, GameObject& obj) {
    Action* action = obj.fsm->current_state->input(world, obj, next_action_type);

    // consume the action
    next_action_type = ActionType::None;
    if (action != nullptr) {
        action->perform(world, obj);
        delete action;
    }
}

void KeyboardInput::collect_discrete_event(SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.repeat == 0) {
        if (event->key.scancode == SDL_SCANCODE_SPACE) {
            next_action_type = ActionType::Jump;
        }
    }
}