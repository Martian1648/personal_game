/*
ὄνομα: camera
αἰών: 2/4/2026
βούλημα: game
*/

#pragma once
#include <vec.h>

#include "gameobject.h"
#include "physics.h"
#include "sprite.h"
#include "tilemap.h"
#include "toggle.h"


class Graphics;
class Color;
class Camera {
public:
    Camera(Graphics& graphics, float tilesize);

    void update(const Vec<float>& new_location, float dt);
    Vec<float> world_to_screen(const Vec<float>& world_position) const;
    void set_location(const Vec<float>& new_location);
    void handle_input();
    void render_loss();
    void render_victory();
    void render(const Vec<float>& position, const Color& color, bool filled=true) const;
    void render(const Tilemap& tilemap) const;
    void render(const Vec<float>& position, const Sprite& sprite , bool flash = false)const;
    void render(const GameObject& obj) const;
    void render(const std::vector<Background>& backgrounds) const;

private:
    Graphics &graphics;
    float tilesize;
    Physics physics;
    Sprite trophy_sprite, skull_sprite;
    void calculate_visible_tiles();
    Vec<int> visible_min, visible_max;
    Vec<float> goal;
    Toggle grid_toggle;
};