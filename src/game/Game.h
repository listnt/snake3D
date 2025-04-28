//
// Created by aidar on 24.04.25.
//

#ifndef GAME_H
#define GAME_H
#include "map.h"
#include "snake.h"

#include <emscripten/html5.h>


class Game {
    snake SnakeI;
    MapInstanse MapI;
    Matrix4x4 targetCamera;
    float dPitch;
    float dYaw;

public:
    Game(int mapSize);

    bool Controls(int eventType, const EmscriptenKeyboardEvent *keyEvent __attribute__((nonnull)), void *userData);

    void GameLoop(Matrix4x4 View);

    void RenderLoop(Matrix4x4 &camera);
};


#endif //GAME_H
