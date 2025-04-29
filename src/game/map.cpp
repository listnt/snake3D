//
// Created by aidar on 21.04.25.
//

#include "map.h"

MapInstanse::MapInstanse(int size, Vector4f color): size(size), color(color) {
    float x = 1.0 / (1.1 * size);

    std::vector<Vector3f> points;
    std::vector<Vector3f> normals;
    std::vector<GLint> triangles;
    std::vector<Vector4f> colors;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < 4; k++) {
                normals.push_back({0, 0, 1});
            }
            for (int k = 0; k < 4; k++) {
                normals.push_back({0, 0, -1});
            }
            for (int k = 0; k < 4; k++) {
                normals.push_back({-1, 0, 0});
            }
            for (int k = 0; k < 4; k++) {
                normals.push_back({1, 0, 0});
            }
            for (int k = 0; k < 4; k++) {
                normals.push_back({0, 1, 0});
            }
            for (int k = 0; k < 4; k++) {
                normals.push_back({0, -1, 0});
            }


            // front
            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x),
                1
            });

            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x + x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x),
                1
            });

            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x + x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x + x),
                1
            });

            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x + x),
                1
            });

            // back
            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x),
                0
            });

            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x + x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x),
                0
            });

            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x + x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x + x),
                0
            });

            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x + x),
                0
            });
            //left
            points.push_back({
                0,
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x),
            });

            points.push_back({
                0,
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x + x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x),
            });

            points.push_back({
                0,
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x + x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x + x),
            });

            points.push_back({
                0,
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x + x),
            });
            // right
            points.push_back({
                1,
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x),
            });

            points.push_back({
                1,
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x + x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x),
            });

            points.push_back({
                1,
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x + x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x + x),
            });

            points.push_back({
                1,
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x),
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x + x),
            });
            // top
            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x),
                1,
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x),
            });

            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x + x),
                1,
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x),
            });

            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x + x),
                1,
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x + x),
            });

            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x),
                1,
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x + x),
            });
            // bottom
            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x),
                0,
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x),
            });

            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x + x),
                0,
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x),
            });

            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x + x),
                0,
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x + x),
            });

            points.push_back({
                static_cast<GLfloat>((2 * i + 1) * (0.05 * x) + i * x),
                0,
                static_cast<GLfloat>((2 * j + 1) * (0.05 * x) + j * x + x),
            });

            auto ci = 6 * 4 * (i * size + j);
            auto t = std::vector<int>();
            for (int k = 0; k < 6; k++) {
                auto cit = ci + 4 * k;
                triangles.insert(triangles.end(), {cit, cit + 1, cit + 2, cit, cit + 2, cit + 3});
            }

            auto c = std::vector<Vector4f>(4 * 6, color);

            colors.insert(colors.end(), c.begin(), c.end());
        }
    }

    auto mapModel = new model(points, colors, triangles, normals);

    loadModel(mapModel);

    delete mapModel;
}
