#pragma once
#include <SDL3/SDL.h>
#include "graphics.h"
#include <utility>
#include "vec.h"

class World;

class Player {
public:
  Player(const Vec<float>& position, const Vec<float>& size);
  void handle_input();
  void update();
  std::pair<SDL_FRect, Color> get_sprite() const;

  // data
  Vec<float> position;
  Vec<float> size;
  Vec<float> velocity;
  Vec<float> acceleration;
};