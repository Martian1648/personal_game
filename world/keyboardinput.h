/*
ὄνομα: keyboard_input
αἰών: 2/25/2026
βούλημα: game
*/

#pragma once
#include "input.h"

class Keyboard_Input : public Input {
public:
    void collect_discrete_event(SDL_Event *event) override;
    void get_input() override;
    void handle_input(World &world, GameObject &obj) override;
};