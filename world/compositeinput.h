/*
ὄνομα: compositeinput
αἰών: 4/28/2026
βούλημα: game
*/

#pragma once

#include "input.h"
#include "keyboardinput.h"
#include "controllerinput.h"

class CompositeInput : public Input {
public:
    CompositeInput();
    ~CompositeInput() override;

    void get_input() override;
    void handle_input(World& world, GameObject& obj) override;
    Action* collect_discrete_event(SDL_Event* event) override;

private:
    Keyboard_Input* keyboard = nullptr;
    ControllerInput* controller = nullptr;
};
