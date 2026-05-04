#pragma once
#include "game_object.h"

class Projectile : public GameObject{
public:
  Projectile(std::string name, FSM* fsm, Input* input, double lifespan=1.0);

  double lifetime;
  double elapsed{0.0};
  int direction;

  void update(World& world, double dt) override;
};