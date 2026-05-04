/*
ὄνομα: camera
αἰών: 2/4/2026
βούλημα: game
*/

#include "camera.h"
#include <cmath>
#include <filesystem>
#include <iostream>

#include "graphics.h"
#include "physics.h"

Camera::Camera(Graphics &graphics, float tilesize):
graphics{graphics}, tilesize{tilesize}{
calculate_visible_tiles();
    physics.damping=0.9;
    auto path = (std::filesystem::current_path() / "assets" / "trophy.png").string();
    trophy_sprite.texture_id = graphics.get_texture_id(path);
    SDL_Texture* tex = graphics.textures.at(trophy_sprite.texture_id);
    float w, h;
    SDL_GetTextureSize(tex, &w, &h);
    trophy_sprite.size = {w, h};
    trophy_sprite.center = trophy_sprite.size / 2.0f;

     path = (std::filesystem::current_path() / "assets" / "skull.png").string();
    skull_sprite.texture_id = graphics.get_texture_id(path);
    tex = graphics.textures.at(skull_sprite.texture_id);
    SDL_GetTextureSize(tex, &w, &h);
    skull_sprite.size = {w, h};
    skull_sprite.center = skull_sprite.size / 2.0f;
}

void Camera::calculate_visible_tiles() {
    Vec<int> num_tiles = Vec{graphics.width, graphics.height}/(2*static_cast<int>(tilesize))+Vec{1,1};
    Vec<int> center{static_cast<int>(physics.position.x), static_cast<int>(physics.position.y)};
    visible_max = center+num_tiles;
    visible_min=center-num_tiles;
}


Vec<float> Camera::world_to_screen(const Vec<float> &world_position) const {
    //world coordinates (+y is up) -> screen coordinates (+y is down)
    Vec<float> pixel = (world_position - physics.position) * static_cast<float>(tilesize);
    //shift to center
    pixel += Vec<float>{graphics.width/2.0f, graphics.height/2.0f};
    //flip y
    pixel.y = graphics.height - pixel.y;
    return pixel;
}

void Camera::handle_input() {
    //TODO: check if g was pressed, if so, call update on the toggle
    const bool* key_states = SDL_GetKeyboardState(NULL);
    if (key_states[SDL_SCANCODE_G]) {
        grid_toggle.flip();
    }
}


void Camera::update(const Vec<float> &new_location, float dt) {
    goal = new_location;
    physics.acceleration = (goal - physics.position) * 10.0f;

    physics.velocity += 0.5f * physics.acceleration * dt;
    physics.position += physics.velocity * dt;
    physics.velocity += .5f * physics.acceleration *dt;
    physics.velocity *= {physics.damping, physics.damping};

    calculate_visible_tiles();
}

void Camera::render(const Vec<float> &position, const Color &color, bool filled) const {
    Vec<float> pixel = world_to_screen(position);
    pixel -= Vec{tilesize/2, tilesize/2};
    SDL_FRect rect{pixel.x, pixel.y, tilesize, tilesize};
    graphics.draw(rect, color, filled);
}

void Camera::render(const Tilemap &tilemap) const {
    int xmin = std::max(0, visible_min.x);
    int ymin = std::max(0, visible_min.y);
    int xmax = std::min(visible_max.x, tilemap.width-1);
    int ymax = std::min(visible_max.y, tilemap.height -1);

    for (int y = ymin; y <= ymax; ++y) {
        for (int x = xmin; x <= xmax; ++x) {
            const Tile& tile = tilemap(x,y);
            Vec<float> position{static_cast<float>(x), static_cast<float>(y)};

            if (tile.blocking) {
                render(position, tile.sprite);
            }
            else {

            }
            if (grid_toggle.on) {
                render(position, {100,100,100,255}, true);
                Color col = {0,0,0, 0};
                render(position, col, false);
            }
        }
    }
}

void Camera::render(const Vec<float> &position, const Sprite &sprite, bool flash) const {
    Vec<float> pixel = world_to_screen(position);
    pixel.y += tilesize/2;
    graphics.draw_sprite(pixel, sprite, flash);
}

void Camera::render(const GameObject &obj) const {
    if (grid_toggle.on == true) {
        render(obj.physics.position, obj.color);
    }



    render(obj.physics.position, obj.sprite, obj.flash_sprite());
}



void Camera::set_location(const Vec<float> & new_location) {
    {
        physics.position = new_location;
        calculate_visible_tiles();
    }
}

void Camera::render(const std::vector<Background>& backgrounds) const {
    for (auto background : backgrounds) {
        const Sprite& sprite = background.sprite;
        float sprite_width = sprite.size.x * sprite.scale;
        if (sprite_width <= 0.0f) continue;

        float shift = physics.position.x / background.distance;
        float wrapped = std::fmod(shift, sprite_width);
        if (wrapped < 0.0f) {
            wrapped += sprite_width;
        }

        float start = -wrapped;
        for (float x = start; x < graphics.width; x += sprite_width) {
            graphics.draw_sprite({x, 0}, sprite);
        }
    }
}


void Camera::render_loss() {
    SDL_FRect full_screen{0.0f, 0.0f,
                           static_cast<float>(graphics.width),
                           static_cast<float>(graphics.height)};
    graphics.draw(full_screen, Color{0, 0, 0, 180}, true);

    Vec<float> pos{
        (graphics.width  - skull_sprite.size.x) / 2.0f,
        (graphics.height - skull_sprite.size.y) / 2.0f
    };
    graphics.draw_sprite(pos, skull_sprite, false);
    std::cout<<"YOU SUCK\n";
}

void Camera::render_victory() {
    SDL_FRect full_screen{0.0f, 0.0f,
                          static_cast<float>(graphics.width),
                          static_cast<float>(graphics.height)};
    graphics.draw(full_screen, Color{0, 0, 0, 180}, true);

    Vec<float> pos{
        (graphics.width  - trophy_sprite.size.x) / 2.0f,
        (graphics.height - trophy_sprite.size.y) / 2.0f
    };
    graphics.draw_sprite(pos, trophy_sprite, false);
    std::cout<<"Well done!\n";
}
