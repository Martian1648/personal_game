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
     Action* input(World &, GameObject &, ActionType) override;
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

class Attacking : public State {
public:
    void on_enter(World &, GameObject &) override;
    void update(World &, GameObject &, double dt) override;
    Action* input(World &, GameObject &, ActionType) override;
    const double cooldown = 0.3;
    double elapsed = 0;
};

class Patroling : public Running {
    void on_enter(World &, GameObject &) override;
    Action* input(World &, GameObject &, ActionType) override;
    void update(World &, GameObject &, double dt) override;
    double cooldown = 2;
    double elapsed = 0;
};
