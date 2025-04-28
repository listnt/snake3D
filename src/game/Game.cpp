//
// Created by aidar on 24.04.25.
//

#include "Game.h"

#include <cstring>

Game::Game(int mapSize): SnakeI(mapSize), MapI(mapSize, {0, 1.0, 1.0, 0.5}) {
    MapI.Transform({
        -static_cast<GLfloat>(mapSize / 2.0),
        -static_cast<GLfloat>(mapSize / 2.0),
        -static_cast<GLfloat>(mapSize / 2.0)
    });
    MapI.Scale({static_cast<GLfloat>(mapSize), static_cast<GLfloat>(mapSize), static_cast<GLfloat>(mapSize)});
}

bool Game::Controls(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
    if (strcmp(keyEvent->key, "ArrowRight") == 0) {
        this->SnakeI.turn(RIGHT);
    } else if (strcmp(keyEvent->key, "ArrowLeft") == 0) {
        this->SnakeI.turn(LEFT);
    } else if (strcmp(keyEvent->key, "ArrowDown") == 0) {
        this->SnakeI.turn(DOWN);
    } else if (strcmp(keyEvent->key, "ArrowUp") == 0) {
        printf("Up\n");
        this->SnakeI.turn(UP);
    }

    return true;
}


void Game::GameLoop(Matrix4x4 camera) {
    SnakeI.move();

    auto headPos = SnakeI.HeadPos();

    auto right = Vector3f(camera.m[0][0], camera.m[0][1], camera.m[0][2]);
    auto forward = Vector3f(camera.m[2][0], camera.m[2][1], camera.m[2][2]);

    auto angleXZ = acos(headPos.dot(forward) / (headPos.length() * forward.length()));
    auto angleZY = acos(headPos.dot(right) / (headPos.length() * right.length()));

    // targetCamera =
    //         pitch(angleXZ) *
    //         camera;
}

void Game::RenderLoop(Matrix4x4 &camera) {
    // camera = targetCamera;

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    SnakeI.Render(inverse(camera));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Turn off updating of the z-buffer
    glDepthMask(GL_FALSE);

    MapI.Render(inverse(camera));
}
