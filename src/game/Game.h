//
// Created by aidar on 24.04.25.
//

#ifndef GAME_H
#define GAME_H
#include "map.h"
#include "snake.h"

#include <emscripten/html5.h>


class Game {
    bool isActive = true;

    snake SnakeI;
    MapInstanse MapI;

    int mapSize;
    float cameraSpeed;
    float t = 0;
    Vector3f initialD = {0, 0, -15};
    Vector3f currentD = {0, 0, -15};
    Vector3f targetD = {0, 0, -15};

    Vector3f initialUp = {0, 1, 0};
    Vector3f currentUp = {0, 1, 0};
    Vector3f targetUp = {0, 1, 0};


    instance apple;
    Vector3i applePos = {100, 100, 100};

public:
    Game(int mapSize);

    bool Controls(int eventType, const EmscriptenKeyboardEvent *keyEvent __attribute__((nonnull)), void *userData);

    void GameLoop(Matrix4x4 &camera);

    void RenderLoop(Matrix4x4 &camera);

    void CreateApple();
};


#endif //GAME_H
