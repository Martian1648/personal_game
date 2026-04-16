#pragma once
#include <SDL3/SDL.h>

#include "animatedsprite.h"
#include "fsm.h"
#include "physics.h"
#include "../graphics/graphics.h"
#include "utility"
#include "../utility/vec.h"
class Input;
class World;
class AABB;
using Sprites = std::map<std::string, AnimatedSprite>;

class GameObject {
public:
    GameObject(std::string name,
        FSM* fsm, Color color, Input* input);
    ~GameObject();
    void update(World& world, double dt);
    std::pair<Vec<float>, Color> get_sprite() const;
    AABB get_bounding_box();
    std::string obj_name;
    void set_sprite(const std::string& next_sprite);
    Physics physics;
    Vec<int> size;
    FSM* fsm;
    Input* input;
    Color color;
    Sprites sprites;
    Sprite sprite;
    std::string sprite_name;
    int health;
    int max_health;
    int damage;
};