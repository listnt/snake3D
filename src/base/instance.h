//
// Created by aidar on 16.04.25.
//

#ifndef INSTANCE_H
#define INSTANCE_H

#include <algorithm>
#include <math.h>
#include <vector>

#include "base.h"

class model {
protected:
    std::vector<Vector3f> points;
    std::vector<Vector4f> colors;
    std::vector<GLint> triangles;

public:
    model(const std::vector<Vector3f> &points, const std::vector<Vector4f> &colors,
          const std::vector<GLint> &triangles): points(points),
                                                triangles(triangles), colors(colors) {
    }

    model() = default;

    std::vector<Vector3f> getPoints() {
        return points;
    }

    std::vector<Vector4f> getColors() {
        return colors;
    }

    std::vector<GLint> getTriangles() {
        return triangles;
    }
};

class instance : public model {
protected:
    bool hasTransparentElements = false;
    GLuint VBO = 0;
    GLuint VAO = 0;
    GLuint EBO = 0;
    GLuint Colors = 0;

    std::vector<float> trianglesZ;

public:
    Vector3f T, S, R;

    instance(): T(0, 0, 0), S(1, 1, 1), R(0, 0, 0) {
    };

    void loadModel(model *obj) {
        points = obj->getPoints();
        triangles = obj->getTriangles();
        colors = obj->getColors();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &Colors);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        for (int i = 0; i < triangles.size() / 3; i++) {
            this->trianglesZ.push_back(0.0);
        }

        for (auto c: colors) {
            if (c.w < 1.0)
                hasTransparentElements = true;
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Vector3f), points.data(),GL_STATIC_DRAW);
        auto position = glGetAttribLocation(userData->programObject, "vPosition");
        glVertexAttribPointer(position, 3, GL_FLOAT, 0, sizeof(Vector3f), 0);
        glEnableVertexAttribArray(position);

        glBindBuffer(GL_ARRAY_BUFFER, Colors);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Vector4f), colors.data(),GL_STATIC_DRAW);
        auto color = glGetAttribLocation(userData->programObject, "vColor");
        glVertexAttribPointer(color, 4,GL_FLOAT, 0, sizeof(Vector4f), 0);
        glEnableVertexAttribArray(color);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(GLint), triangles.data(),
                     GL_DYNAMIC_DRAW);


        glBindVertexArray(0);
    }

    ~instance() {
        printf("Delete");
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &Colors);
        glDeleteBuffers(1, &VAO);
    }

    void Render(Matrix4x4 View);

    void Transform(Vector3f T);

    void Rotate(Vector3f R);

    void Scale(Vector3f S);

    bool HasTransparentElements() const {
        return hasTransparentElements;
    }

    void ReorderRenderForTransparent(Matrix4x4 ModelView);
};

model *createCube(Vector4f color);

#endif //INSTANCE_H
