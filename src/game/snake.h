//
// Created by aidar on 24.04.25.
//

#ifndef SNAKE_H
#define SNAKE_H
#include <vector>

#include "../base/base.h"
#include "../base/instance.h"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class snake {
private:
    int mapSize;
    Direction d;
    Vector3i Right;
    Vector3i Normal;
    Vector3i Forward;
    Vector3i step;
    std::vector<Vector3i> segmentsPos;
    std::vector<instance *> cubes;

public:
    snake(int mapSize);

    void turn(Direction newDirection);

    void Render(Matrix4x4 View);

    Vector3f HeadPos();

    void move();

    bool passesRightSide(Vector3i pos, int posOnGridX, int posOnGridY);

    bool passesLeftSide(Vector3i pos, int posOnGridX, int posOnGridY);

    bool passesTopSide(Vector3i pos, int posOnGridX, int posOnGridY);

    bool passesBottomSide(Vector3i pos, int posOnGridX, int posOnGridY);
};


#endif //SNAKE_H
