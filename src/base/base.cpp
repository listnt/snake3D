//
// Created by aidar on 20.04.25.
//

#include "base.h"

#include <cstdio>

Matrix4x4 scale(double x, double y, double z) {
    Matrix4x4 S = Matrix4x4();
    S.m[0][0] = x;
    S.m[1][1] = y;
    S.m[2][2] = z;
    S.m[3][3] = 1;

    return S;
}

Matrix4x4 roll(double angle) {
    Matrix4x4 R = Matrix4x4();
    R.m[0][0] = 1;
    R.m[1][1] = cos(angle);
    R.m[1][2] = -sin(angle);
    R.m[2][1] = sin(angle);
    R.m[2][2] = cos(angle);
    R.m[3][3] = 1;

    return R;
}

Matrix4x4 pitch(double angle) {
    Matrix4x4 R = Matrix4x4();
    R.m[0][0] = cos(angle);
    R.m[0][2] = sin(angle);
    R.m[1][1] = 1;
    R.m[2][0] = -sin(angle);
    R.m[2][2] = cos(angle);
    R.m[3][3] = 1;

    return R;
}

Matrix4x4 yaw(double angle) {
    Matrix4x4 R = Matrix4x4();
    R.m[0][0] = cos(angle);
    R.m[0][1] = -sin(angle);
    R.m[1][0] = sin(angle);
    R.m[1][1] = cos(angle);
    R.m[2][2] = 1;
    R.m[3][3] = 1;

    return R;
}

Matrix4x4 translate(double x, double y, double z) {
    Matrix4x4 T = Matrix4x4();
    T.m[0][3] = x;
    T.m[1][3] = y;
    T.m[2][3] = z;
    T.m[0][0] = 1;
    T.m[1][1] = 1;
    T.m[2][2] = 1;
    T.m[3][3] = 1;

    return T;
}

std::vector<GLfloat> flatten(Matrix4x4 m) {
    std::vector<GLfloat> v;
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            v.push_back(m.m[i][j]);
        }
    }
    return v;
}

Matrix4x4 computePerspectiveMatrix(float fovInRads, float aspectRatio, float near, float far) {
    Matrix4x4 perspective = Matrix4x4();

    auto f = 1.0 / tan(fovInRads / 2);
    auto rangeinv = 1.0 / (near - far);

    perspective.m[0][0] = f / aspectRatio;
    perspective.m[1][1] = f;
    perspective.m[2][2] = (near + far) * rangeinv;\
    perspective.m[2][3] = -1;
    perspective.m[3][2] = 2 * near * far * rangeinv;

    return perspective;
}

Matrix4x4 lookAt(Vector3f eye, Vector3f center, Vector3f u) {
    Matrix4x4 lookAt = Matrix4x4();
    Vector3f forward = (center - eye).normalize();
    Vector3f right = forward.cross(u).normalize();
    Vector3f up = right.cross(forward).normalize();

    lookAt.m[0][0] = right.x;
    lookAt.m[1][0] = up.x;
    lookAt.m[2][0] = forward.x;
    lookAt.m[0][1] = right.y;
    lookAt.m[1][1] = up.y;
    lookAt.m[2][1] = forward.y;
    lookAt.m[0][2] = right.z;
    lookAt.m[1][2] = up.z;
    lookAt.m[2][2] = forward.z;
    lookAt.m[3][0] = 0;
    lookAt.m[3][1] = 0;
    lookAt.m[3][2] = 0;
    lookAt.m[3][3] = 1;

    return lookAt * translate(eye.x, eye.y, eye.z);
}

Matrix4x4 inverse(Matrix4x4 m) {
    /* Structure of matrix

         0   1   2   3
        ______________
     0 | 0   4   8  12
     1 | 1   5   9  13
     2 | 2   6  10  14
     3 | 3   7  11  15
    */

    // Factored out common terms
    auto t9_14_13_10 = m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2];
    auto t13_6_5_14 = m.m[1][3] * m.m[2][1] - m.m[1][1] * m.m[2][3];
    auto t5_10_9_6 = m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1];
    auto t12_10_8_14 = m.m[0][3] * m.m[2][2] - m.m[0][2] * m.m[2][3];
    auto t4_14_12_6 = m.m[0][1] * m.m[2][3] - m.m[0][3] * m.m[2][1];
    auto t8_6_4_10 = m.m[0][2] * m.m[2][1] - m.m[0][1] * m.m[2][2];
    auto t8_13_12_9 = m.m[0][2] * m.m[1][3] - m.m[0][3] * m.m[1][2];
    auto t12_5_4_13 = m.m[0][3] * m.m[1][1] - m.m[0][1] * m.m[1][3];
    auto t4_9_8_5 = m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1];
    auto t1_14_13_2 = m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0];
    auto t9_2_1_10 = m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2];
    auto t12_2_0_14 = m.m[0][3] * m.m[2][0] - m.m[0][0] * m.m[2][3];
    auto t0_10_8_2 = m.m[0][0] * m.m[2][2] - m.m[0][2] * m.m[2][0];
    auto t0_13_12_1 = m.m[0][0] * m.m[1][3] - m.m[0][3] * m.m[1][0];
    auto t8_1_0_9 = m.m[0][2] * m.m[1][0] - m.m[0][0] * m.m[1][2];
    auto t1_6_5_2 = m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0];
    auto t4_2_0_6 = m.m[0][1] * m.m[2][0] - m.m[0][0] * m.m[2][1];
    auto t0_5_4_1 = m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0];

    Matrix4x4 Inv;
    Inv.m[0][0] = m.m[3][1] * t9_14_13_10 + m.m[3][2] * t13_6_5_14 + m.m[3][3] * t5_10_9_6;
    Inv.m[0][1] = m.m[3][1] * t12_10_8_14 + m.m[3][2] * t4_14_12_6 + m.m[3][3] * t8_6_4_10;
    Inv.m[0][2] = m.m[3][1] * t8_13_12_9 + m.m[3][2] * t12_5_4_13 + m.m[3][3] * t4_9_8_5;
    Inv.m[0][3] = m.m[2][1] * (-t8_13_12_9) + m.m[2][2] * (-t12_5_4_13) + m.m[2][3] * (-t4_9_8_5);
    Inv.m[1][0] = m.m[3][0] * (-t9_14_13_10) + m.m[3][2] * t1_14_13_2 + m.m[3][3] * t9_2_1_10;
    Inv.m[1][1] = m.m[3][0] * (-t12_10_8_14) + m.m[3][2] * t12_2_0_14 + m.m[3][3] * t0_10_8_2;
    Inv.m[1][2] = m.m[3][0] * (-t8_13_12_9) + m.m[3][2] * t0_13_12_1 + m.m[3][3] * t8_1_0_9;
    Inv.m[1][3] = m.m[2][0] * t8_13_12_9 + m.m[2][2] * (-t0_13_12_1) + m.m[2][3] * (-t8_1_0_9);
    Inv.m[2][0] = m.m[3][0] * (-t13_6_5_14) + m.m[3][1] * (-t1_14_13_2) + m.m[3][3] * t1_6_5_2;
    Inv.m[2][1] = m.m[3][0] * (-t4_14_12_6) + m.m[3][1] * (-t12_2_0_14) + m.m[3][3] * t4_2_0_6;
    Inv.m[2][2] = m.m[3][0] * (-t12_5_4_13) + m.m[3][1] * (-t0_13_12_1) + m.m[3][3] * t0_5_4_1;
    Inv.m[2][3] = m.m[2][0] * t12_5_4_13 + m.m[2][1] * t0_13_12_1 + m.m[2][3] * (-t0_5_4_1);
    Inv.m[3][0] = m.m[3][0] * (-t5_10_9_6) + m.m[3][1] * (-t9_2_1_10) + m.m[3][2] * (-t1_6_5_2);
    Inv.m[3][1] = m.m[3][0] * (-t8_6_4_10) + m.m[3][1] * (-t0_10_8_2) + m.m[3][2] * (-t4_2_0_6);
    Inv.m[3][2] = m.m[3][0] * (-t4_9_8_5) + m.m[3][1] * (-t8_1_0_9) + m.m[3][2] * (-t0_5_4_1);
    Inv.m[3][3] = m.m[2][0] * t4_9_8_5 + m.m[2][1] * t8_1_0_9 + m.m[2][2] * t0_5_4_1;

    auto det =
            m.m[3][0] * (m.m[2][1] * (-t8_13_12_9) + m.m[2][2] * (-t12_5_4_13) + m.m[2][3] * (-t4_9_8_5)) +
            m.m[3][1] * (m.m[2][0] * t8_13_12_9 + m.m[2][2] * (-t0_13_12_1) + m.m[2][3] * (-t8_1_0_9)) +
            m.m[3][2] * (m.m[2][0] * t12_5_4_13 + m.m[2][1] * t0_13_12_1 + m.m[2][3] * (-t0_5_4_1)) +
            m.m[3][3] * (m.m[2][0] * t4_9_8_5 + m.m[2][1] * t8_1_0_9 + m.m[2][2] * t0_5_4_1);

    if (det != 0) {
        auto scale = 1 / det;
        for (int j = 0; j < 16; j += 1) {
            Inv.m[j / 4][j % 4] = Inv.m[j / 4][j % 4] * scale;
        }
    }

    return Inv;
}
