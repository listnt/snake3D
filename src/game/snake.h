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
    std::vector<bool> isActive;
    std::vector<Vector3i> segmentsPos;
    std::vector<instance *> cubes;

    model *redCube;
    model *greenCube;

public:
    snake(int mapSize);

    void turn(Direction newDirection);

    void Render(Matrix4x4 View);

    Vector3f HeadPos();

    Vector3i HeadPosOnGrid();

    Vector3f GetRight();

    Vector3f GetForward();

    Vector3f GetNormal();

    std::vector<Vector3i> getSegmentsPos();

    bool isOccupied(Vector3i pos);

    bool move();

    bool passesRightSide(Vector3i pos, int posOnGridX, int posOnGridY);

    bool passesLeftSide(Vector3i pos, int posOnGridX, int posOnGridY);

    bool passesTopSide(Vector3i pos, int posOnGridX, int posOnGridY);

    bool passesBottomSide(Vector3i pos, int posOnGridX, int posOnGridY);

    void grow(int size);
};


#endif //SNAKE_H
