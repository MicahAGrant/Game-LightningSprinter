#include "events.h"
#include "world.h"
#include"game_object.h"

#include <iostream>

void NextLevel::perform(World& world, GameObject& obj) {
    world.end_level = true;
}