/*
ὄνομα: composite_input
αἰών: 4/28/2026
βούλημα: game
*/

#include "compositeinput.h"

CompositeInput::CompositeInput() : keyboard(new Keyboard_Input()) {}

CompositeInput::~CompositeInput() {
    delete keyboard;
    delete controller;
}

void CompositeInput::get_input() {
    if (next_action_type == ActionType::Jump || next_action_type == ActionType::Attack) {
        return;
    }

    if (controller) {
        controller->next_action_type = ActionType::None;
        controller->get_input();
        if (controller->next_action_type != ActionType::None) {
            next_action_type = controller->next_action_type;
            return;
        }
    }

    keyboard->next_action_type = ActionType::None;
    keyboard->get_input();
    next_action_type = keyboard->next_action_type;
}

void CompositeInput::handle_input(World& world, GameObject& obj) {
    Action* action = obj.fsm->current_state->input(world, obj, next_action_type);
    next_action_type = ActionType::None;
    if (action != nullptr) {
        action->perform(world, obj);
        delete action;
    }
}

Action* CompositeInput::collect_discrete_event(SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP) {
        Action* action = keyboard->collect_discrete_event(event);
        if (keyboard->next_action_type != ActionType::None) {
            next_action_type = keyboard->next_action_type;
            keyboard->next_action_type = ActionType::None;
        }
        return action;
    }
    else if (event->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN || event->type == SDL_EVENT_GAMEPAD_BUTTON_UP) {
        if (!controller) return nullptr;
        Action* action = controller->collect_discrete_event(event);
        if (controller->next_action_type != ActionType::None) {
            next_action_type = controller->next_action_type;
            controller->next_action_type = ActionType::None;
        }
        return action;
    }
    else if (event->type == SDL_EVENT_GAMEPAD_ADDED) {
        if (!controller) {
            controller = new ControllerInput(event->gdevice.which);
        }
    }
    else if (event->type == SDL_EVENT_GAMEPAD_REMOVED) {
        if (controller && controller->matches(event->gdevice.which)) {
            delete controller;
            controller = nullptr;
        }
    }
    return nullptr;
}