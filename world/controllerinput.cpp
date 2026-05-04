/*
ὄνομα: ControllerInput
αἰών: 4/28/2026
βούλημα: game
*/

#include "controllerinput.h"
#include "action.h"
#include "projectile.h"

ControllerInput::ControllerInput(SDL_JoystickID id) : instance_id(id) {
    pad = SDL_OpenGamepad(id);
    if (!pad) {
        throw std::runtime_error("No pad!");
    }
}

ControllerInput::~ControllerInput() {
    if (pad) SDL_CloseGamepad(pad);
}

void ControllerInput::get_input() {
    if (next_action_type == ActionType::Jump || next_action_type == ActionType::Attack) {
        return;
    }

    bool sprinting = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER) || SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);

    if (SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT)) {
        next_action_type = sprinting ? ActionType::SprintRight : ActionType::MoveRight;
    } else if (SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_DPAD_LEFT)) {
        next_action_type = sprinting ? ActionType::SprintLeft : ActionType::MoveLeft;
    }
}

void ControllerInput::handle_input(World& world, GameObject& obj) {
    Action* action = obj.fsm->current_state->input(world, obj, next_action_type);
    next_action_type = ActionType::None;
    if (action != nullptr) {
        action->perform(world, obj);
        delete action;
    }
}

Action* ControllerInput::collect_discrete_event(SDL_Event* event) {
    if (event->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN && event->gbutton.which == instance_id) {
        if (event->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
            next_action_type = ActionType::Jump;
        }
        else if (event->gbutton.button == SDL_GAMEPAD_BUTTON_WEST) {
            next_action_type = ActionType::Attack;
        }
        else if (event->gbutton.button == SDL_GAMEPAD_BUTTON_NORTH) {
            return new ShootFireball();
        }
    }
    return nullptr;
}