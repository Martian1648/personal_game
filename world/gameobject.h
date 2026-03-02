#pragma once
#include <SDL3/SDL.h>

#include "fsm.h"
#include "physics.h"
#include "../graphics/graphics.h"
#include "utility"
#include "../utility/vec.h"
class Input;
class World;

class GameObject {
public:
    GameObject(const Vec<float>& position, const Vec<int> &size, World& world,
        FSM* fsm, Color color, Input* input);
    ~GameObject();
    void update(World& world, double dt);
    std::pair<Vec<float>, Color> get_sprite() const;


    Physics physics;
    Vec<int> size;
    FSM* fsm;
    Input* input;
    Color color;

    Sprite sprite;
};