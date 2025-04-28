//
// Created by aidar on 24.04.25.
//

#include "snake.h"

snake::snake(int mapSize) {
    this->mapSize = mapSize;
    segmentsPos.push_back({mapSize / 2 + 1, mapSize / 2 + 1, 0});

    auto cube = createCube({0, 1, 0, 1});

    auto cubeI = new instance();
    cubeI->loadModel(cube);

    cubes.push_back(cubeI);

    Right = Vector3i({1, 0, 0});
    Normal = Vector3i({0, 0, -1});
    Forward = Vector3i({0, 1, 0});

    step = Vector3i({1, 0, 0});
    d = RIGHT;
}

void snake::turn(Direction newDirection) {
    this->d = newDirection;

    switch (newDirection) {
        case RIGHT:
            step = Right;
            break;
        case LEFT:
            step = -Right;
            break;
        case UP:
            step = Forward;
            break;
        case DOWN:
            step = -Forward;
            break;
    }
}

void snake::Render(Matrix4x4 View) {
    for (int i = 0; i < segmentsPos.size(); i++) {
        cubes[i]->Transform({
            static_cast<GLfloat>(segmentsPos[i].i - (int) (mapSize + 2) / 2),
            static_cast<GLfloat>(segmentsPos[i].j - (int) (mapSize + 2) / 2),
            static_cast<GLfloat>(segmentsPos[i].k - (int) (mapSize + 2) / 2)
        });
        cubes[i]->Render(View);
    }
}

Vector3f snake::HeadPos() {
    return {segmentsPos[0]};
}

void snake::move() {
    for (auto &pos: segmentsPos) {
        pos = pos + step;
        auto posOnGridX = abs(pos.i * Right.i + pos.j * Right.j + pos.k * Right.k);
        auto posOnGridY = abs(pos.i * Forward.i + pos.j * Forward.j + pos.k * Forward.k);
        if (passesRightSide(pos, posOnGridX, posOnGridY)) {
            pos = pos + step;
            step = -Normal;

            auto tmpX = Right;
            Right = -Normal;
            Normal = tmpX;

            pos = pos + step;
        } else if (passesLeftSide(pos, posOnGridX, posOnGridY)) {
            pos = pos + step;
            step = -Normal;

            auto tmpX = Right;
            Right = Normal;
            Normal = -tmpX;

            pos = pos + step;
        } else if (passesTopSide(pos, posOnGridX, posOnGridY)) {
            pos = pos + step;
            step = -Normal;

            auto tmpY = Forward;
            Forward = -Normal;
            Normal = tmpY;

            pos = pos + step;
        } else if (passesBottomSide(pos, posOnGridX, posOnGridY)) {
            pos = pos + step;
            step = -Normal;

            auto tmpY = Forward;
            Forward = Normal;
            Normal = -tmpY;

            pos = pos + step;
        }
    }
}

bool snake::passesRightSide(Vector3i pos, int posOnGridX, int posOnGridY) {
    if (step.i == Right.i && step.j == Right.j && step.k == Right.k && d == RIGHT) {
        if (posOnGridX == mapSize || posOnGridX <= 1) {
            return true;
        }
    }

    return false;
}

bool snake::passesLeftSide(Vector3i pos, int posOnGridX, int posOnGridY) {
    if (step.i == -Right.i && step.j == -Right.j && step.k == -Right.k && d == LEFT) {
        if (posOnGridX == mapSize || posOnGridX <= 1) {
            return true;
        }
    }

    return false;
}

bool snake::passesTopSide(Vector3i pos, int posOnGridX, int posOnGridY) {
    if (step.i == Forward.i && step.j == Forward.j && step.k == Forward.k && d == UP) {
        if (posOnGridY == mapSize || posOnGridY <= 1) {
            return true;
        }
    }

    return false;
}


bool snake::passesBottomSide(Vector3i pos, int posOnGridX, int posOnGridY) {
    if (step.i == -Forward.i && step.j == -Forward.j && step.k == -Forward.k && d == DOWN) {
        if (posOnGridY == mapSize || posOnGridY <= 1) {
            return true;
        }
    }

    return false;
}
