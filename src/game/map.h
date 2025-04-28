//
// Created by aidar on 21.04.25.
//

#ifndef MAP_H
#define MAP_H

#include "../base/instance.h"


class MapInstanse : public instance {
protected:
    int size;
    Vector4f color;

public:
    MapInstanse(int size, Vector4f color);
};


#endif //MAP_H
