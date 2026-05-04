/*
ὄνομα: controllerinput
αἰών: 4/28/2026
βούλημα: game
*/

#pragma once
#include "input.h"

class ControllerInput : public Input{
public:
    ControllerInput(SDL_JoystickID id);
    ~ControllerInput() override;
    Action* collect_discrete_event(SDL_Event *event) override;
    void get_input() override;
    void handle_input(World &world, GameObject &obj) override;
    bool matches(SDL_JoystickID id) const { return id == instance_id; }

private:
    SDL_Gamepad* pad = nullptr;
    SDL_JoystickID instance_id;
};
