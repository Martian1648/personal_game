/*
ὄνομα: events
αἰών: 3/25/2026
βούλημα: game
*/

#include "events.h"

#include <iostream>

#include "world.h"
#include "gameobject.h"

void NextLevel::perform(World &world, GameObject &obj) {
    if (&obj != world.player) return;
    world.end_level = true;
}

void EndGame::perform(World &world, GameObject &obj) {
    if (&obj != world.player) return;
    world.end_game = true;
}
