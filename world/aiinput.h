/*
ὄνομα: ai_input
αἰών: 4/24/2026
βούλημα: game
*/

#pragma once
#include "input.h"
class AIinput : public Input{
    void get_input() override;
    void handle_input(World &world, GameObject &obj) override;
};