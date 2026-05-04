project(world)

add_library(world
world.cpp world.h
        tilemap.cpp
        tilemap.h
        camera.cpp
        camera.h
        gameobject.cpp
        gameobject.h
        action.cpp
        action.h
        fsm.cpp
        fsm.h
        states.cpp
        states.h
        input.cpp
        input.h
        keyboardinput.cpp
        keyboardinput.h
        level.cpp
        level.h
        audio.cpp
        audio.h
        events.cpp
        events.h
        quadtree.cpp
        quadtree.h
        projectile.cpp
        projectile.h
        aiinput.h
        aiinput.cpp
        controllerinput.cpp
        controllerinput.h
        compositeinput.cpp
        compositeinput.h
)

target_include_directories(world PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(world PUBLIC SDL3::SDL3 SDL3_mixer::SDL3_mixer  utility graphics)