#pragma once
#include "game_object.h"

class AiInput : public Input {
public:
    void get_input() override;
    void handle_input(World& world, GameObject& obj) override;
};
