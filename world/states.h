/*
ὄνομα: states
αἰών: 2/16/2026
βούλημα: game
*/

#pragma once
#include "fsm.h"

class Standing : public State {
public:
     void on_enter(World &, GameObject &) override;
     Action* input(World &, GameObject &, ActionType) override;
};

class InAir : public State {
public:
     void on_enter(World &, GameObject &) override;
     void update(World &, GameObject &, double dt) override;

    const double cooldown = .1;
    double elapsed = 0;
};

class Running : public State {
public:
     void on_enter(World &, GameObject &) override;
    Action* input(World &, GameObject &, ActionType) override;
};

class Sprinting : public State {
public:
    void on_enter(World &, GameObject &obj) override;
    void update(World &, GameObject &, double dt) override;

    const double cooldown = .8;
    double elapsed = 0;
};