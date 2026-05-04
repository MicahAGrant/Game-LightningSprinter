#include "ai_input.h"

#include "fsm.h"
#include "game_object.h"
#include "world.h"

void AiInput::get_input() {}

void AiInput::handle_input(World& world, GameObject& obj) {
    constexpr float epsilon = 1e-4;

    // If currently moving right
    if (next_action_type == ActionType::MoveRight) {

        // wall in front OR no ground ahead then turn around
        if (
            world.collides({obj.physics.position.x + obj.size.x + epsilon, obj.physics.position.y + obj.size.y - epsilon}) ||
                            !world.collides({obj.physics.position.x + obj.size.x + epsilon, obj.physics.position.y - epsilon})
        ) {
            next_action_type = ActionType::MoveLeft;
        }
    }

    // If currently moving left
    else if (next_action_type == ActionType::MoveLeft) {

        // wall in front OR no ground ahead then turn around
        if (
            world.collides({obj.physics.position.x - epsilon, obj.physics.position.y + obj.size.y - epsilon}) ||
                            !world.collides({obj.physics.position.x - epsilon, obj.physics.position.y - epsilon})
        ) {
            next_action_type = ActionType::MoveRight;
        }
    }

    if (obj.obj_name == "void-monster") {
        next_action_type = ActionType::Attack;
    }

    Action* action = obj.fsm->current_state->input(world, obj, next_action_type);

    if (action) {
        action->perform(world, obj);
        delete action;
    }
}