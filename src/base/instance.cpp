//
// Created by aidar on 16.04.25.
//

#include "instance.h"

#include <cstring>
#include <numeric>


#include <chrono> // Import the ctime library


// void instanse::Rotate(Vector3f R) { this->R = this->R + R; }

void instance::Render(Matrix4x4 View) {
    glBindVertexArray(this->VAO);

    auto ViewModel = View * translate(T.x, T.y, T.z) *
                     roll(R.z) * pitch(R.y) * yaw(R.x)
                     * scale(S.x, S.y, S.z);


    if (hasTransparentElements) {
        ReorderRenderForTransparent(ViewModel);
    }

    glUniformMatrix4fv(ViewModelMat, 1,GL_FALSE, flatten(ViewModel).data());

    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void instance::Transform(Vector3f T) { this->T = T; }

void instance::Rotate(Vector3f R) { this->R = R; }

void instance::Scale(Vector3f S) { this->S = S; }

void instance::ReorderRenderForTransparent(Matrix4x4 ProjectionViewModel) {
    for (int i = 0; i < this->trianglesZ.size(); i++) {
        auto whichTriangle = i * 3;
        float maxZ = 10e10;
        Vector3f centroid;
        for (int j = 0; j < 3; j++) {
            centroid = centroid + points[triangles[whichTriangle + j]] / 3.0f;
        }

        trianglesZ[i] = (ProjectionViewModel * Vector4f(centroid, 1.0)).z;
    }


    bool reorderingOccured = false;

    std::vector<int> indices(trianglesZ.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::sort(indices.begin(), indices.end(), [&](int a, int b) {
        return trianglesZ[a] < trianglesZ[b];
    });

    // Fix all elements one by one
    for (int i = 0; i < indices.size(); i++) {
        // While index[i] and arr[i] are not fixed
        while (indices[i] != i) {
            reorderingOccured = true;
            // Store values of the target (or correct)
            // position before placing arr[i] there
            int oldTargetI = indices[indices[i]];

            int oldTargetE1 = triangles[indices[i] * 3];
            int oldTargetE2 = triangles[indices[i] * 3 + 1];
            int oldTargetE3 = triangles[indices[i] * 3 + 2];

            // Place arr[i] at its target (or correct)
            // position. Also copy corrected index for
            // new position
            triangles[indices[i] * 3] = triangles[i * 3];
            triangles[indices[i] * 3 + 1] = triangles[i * 3 + 1];
            triangles[indices[i] * 3 + 2] = triangles[i * 3 + 2];

            indices[indices[i]] = indices[i];

            // Copy old target values to arr[i] and
            // index[i]
            indices[i] = oldTargetI;

            triangles[i * 3] = oldTargetE1;
            triangles[i * 3 + 1] = oldTargetE2;
            triangles[i * 3 + 2] = oldTargetE3;
        }
    }

    if (reorderingOccured) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(GLint), triangles.data(),
                     GL_DYNAMIC_DRAW);
    }
}

model *createCube(Vector4f color) {
    std::vector<Vector4f> colors;
    for (int i = 0; i < 24; i++) {
        colors.push_back(color);
    }

    const auto cube = new model(
        {
            // Front face
            {-0.5, -0.5, 0.5},
            {0.5, -0.5, 0.5},
            {0.5, 0.5, 0.5},
            {-0.5, 0.5, 0.5},

            // Back face
            {-0.5, -0.5, -0.5},
            {-0.5, 0.5, -0.5},
            {0.5, 0.5, -0.5},
            {0.5, -0.5, -0.5},

            // Top face
            {-0.5, 0.5, -0.5},
            {-0.5, 0.5, 0.5},
            {0.5, 0.5, 0.5},
            {0.5, 0.5, -0.5},

            // Bottom face
            {-0.5, -0.5, -0.5},
            {0.5, -0.5, -0.5},
            {0.5, -0.5, 0.5},
            {-0.5, -0.5, 0.5},

            // Right face
            {0.5, -0.5, -0.5},
            {0.5, 0.5, -0.5},
            {0.5, 0.5, 0.5},
            {0.5, -0.5, 0.5},

            // Left face
            {-0.5, -0.5, -0.5},
            {-0.5, -0.5, 0.5},
            {-0.5, 0.5, 0.5},
            {-0.5, 0.5, -0.5}
        },
        colors,
        {
            0, 1, 2, 0, 2, 3, // front
            4, 5, 6, 4, 6, 7, // back
            8, 9, 10, 8, 10, 11, // top
            12, 13, 14, 12, 14, 15, // bottom
            16, 17, 18, 16, 18, 19, // right
            20, 21, 22, 20, 22, 23 // left
        }, {
            // Front face
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},

            // Back face
            {0, 0, -1},
            {0, 0, -1},
            {0, 0, -1},
            {0, 0, -1},

            // Top face
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},

            // Bottom face
            {0, -1, 0},
            {0, -1, 0},
            {0, -1, 0},
            {0, -1, 0},

            // Right face
            {1, 0, 0},
            {1, 0, 0},
            {1, 0, 0},
            {1, 0, 0},

            // Left face
            {-1, 0, 0},
            {-1, 0, 0},
            {-1, 0, 0},
            {-1, 0, 0},
        }
    );

    return cube;
}
