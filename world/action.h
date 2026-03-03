#pragma once

enum class ActionType {
    None,
    Jump,
    MoveLeft,
    MoveRight,
    BoostLeft,
    BoostRight,
    WallJumpLeft,
    WallJumpRight
};

class World;
class GameObject;

class Action {
public:
    virtual ~Action() = default;
    virtual void perform(World& world, GameObject& obj) = 0;
};

class Jump : public Action {
    void perform(World&, GameObject& obj) override;
};

class MoveRight : public Action {
    void perform(World&, GameObject& obj) override;
};

class MoveLeft : public Action {
    void perform(World&, GameObject& obj) override;
};

class BoostRight : public Action {
    void perform(World&, GameObject& obj) override;
};

class BoostLeft : public Action {
    void perform(World&, GameObject& obj) override;
};

class WallJumpLeft : public Action {
    void perform(World&, GameObject& obj) override;
};

class WallJumpRight : public Action {
    void perform(World&, GameObject& obj) override;
};