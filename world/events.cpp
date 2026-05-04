#include "events.h"
#include "world.h"
#include"game_object.h"

#include <iostream>

void NextLevel::perform(World& world, GameObject& obj) {
    if (&obj != world.player) return;
    world.end_level = true;
}

void Win::perform(World& world, GameObject& obj) {
    if (&obj != world.player) return;
    world.end_game = true;
}
