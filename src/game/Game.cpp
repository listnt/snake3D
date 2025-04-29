//
// Created by aidar on 24.04.25.
//

#include "Game.h"

#include <cstring>

Game::Game(int mapSize): SnakeI(mapSize), MapI(mapSize, {0, 1.0, 1.0, 0.5}), mapSize(mapSize) {
    MapI.Transform({
        -static_cast<GLfloat>(mapSize / 2.0),
        -static_cast<GLfloat>(mapSize / 2.0),
        -static_cast<GLfloat>(mapSize / 2.0)
    });
    MapI.Scale({static_cast<GLfloat>(mapSize), static_cast<GLfloat>(mapSize), static_cast<GLfloat>(mapSize)});

    auto appleModel = createCube({1, 0, 0, 1});
    apple.loadModel(appleModel);
    delete appleModel;

    applePos = {mapSize / 2 + 1, 0, (int) mapSize / 2 + 1};

    apple.Scale({0.9, 0.9, 0.9});
    apple.Transform({
        static_cast<GLfloat>(applePos.i - (int) (mapSize + 2) / 2),
        static_cast<GLfloat>(applePos.j - (int) (mapSize + 2) / 2),
        static_cast<GLfloat>(applePos.k - (int) (mapSize + 2) / 2)
    });

    // CreateApple();
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


void Game::GameLoop(Matrix4x4 &camera) {
    if (!isActive)
        return;

    isActive = SnakeI.move();

    auto headPos = SnakeI.HeadPos();

    auto headPosOnGrid = SnakeI.HeadPosOnGrid();

    if (headPosOnGrid == applePos) {
        CreateApple();
        SnakeI.grow(1);
    }

    targetD = headPos.normalize() * 15;
    initialD = currentD;

    targetUp = SnakeI.GetForward();
    initialUp = currentUp;

    t = 0;
}

void Game::RenderLoop(Matrix4x4 &camera) {
    t += 0.01;
    t = std::clamp(t, 0.0f, 1.0f);

    if (t < 0.95) {
        currentD = Lerp(initialD, targetD, t);
        currentUp = Lerp(initialUp, targetUp, t);
    } else {
        currentD = targetD;
        currentUp = targetUp;
    }

    camera = lookAt(currentD, {0, 0, 0}, currentUp);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    SnakeI.Render(camera);
    apple.Render(camera);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Turn off updating of the z-buffer
    glDepthMask(GL_FALSE);

    MapI.Render(camera);

    glDisable(GL_BLEND);
}

void Game::CreateApple() {
    while (SnakeI.isOccupied(applePos)) {
        auto x = rand() % (mapSize) + 1;
        auto y = rand() % (mapSize) + 1;
        auto z = rand() % 6;

        switch (z) {
            case 0:
                applePos = {x, y, 0};
                break;
            case 1:
                applePos = {x, y, mapSize + 1};
                break;
            case 2:
                applePos = {x, 0, y};
                break;
            case 3:
                applePos = {x, mapSize + 1, y};
                break;
            case 4:
                applePos = {0, x, y};
                break;
            case 5:
                applePos = {mapSize + 1, x, y};
                break;
        }
    }

    apple.Transform({
        static_cast<GLfloat>(applePos.i - (int) (mapSize + 2) / 2),
        static_cast<GLfloat>(applePos.j - (int) (mapSize + 2) / 2),
        static_cast<GLfloat>(applePos.k - (int) (mapSize + 2) / 2)
    });

    return;
}
