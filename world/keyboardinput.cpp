/*
ὄνομα: keyboard_input
αἰών: 2/25/2026
βούλημα: game
*/

#include "keyboardinput.h"

void Keyboard_Input::get_input() {
    if (next_action_type == ActionType::Jump) {
        return;
    }
    const bool *key_states = SDL_GetKeyboardState(NULL);
    if (key_states[SDL_SCANCODE_D]) {
        next_action_type = ActionType::MoveRight;
        if (key_states[SDL_SCANCODE_LSHIFT]) {
            next_action_type = ActionType::SprintRight;
        }
    }
    else if (key_states[SDL_SCANCODE_A]) {
        next_action_type = ActionType::MoveLeft;
        if (key_states[SDL_SCANCODE_LSHIFT]) {
            next_action_type = ActionType::SprintLeft;
        }
    }


}

void Keyboard_Input::handle_input(World &world, GameObject &obj) {
    Action* action = obj.fsm->current_state->input(world, obj, next_action_type);

    next_action_type= ActionType::None;
    if (action != nullptr) {
        action->perform(world, obj);
        delete action;
    }
}

void Keyboard_Input::collect_discrete_event(SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.repeat==0) {
        if (event->key.scancode == SDL_SCANCODE_SPACE) {
            next_action_type = ActionType::Jump;
        }
    }
}


