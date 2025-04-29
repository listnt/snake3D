//
// Created by aidar on 19.04.25.
//

#ifndef BASE_H
#define BASE_H
#include <emscripten/html5.h>
#include <emscripten.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <vector>
#include <math.h>

typedef struct {
    // Handle to a program object
    GLuint programObject;
    GLsizei WindowWidth;
    GLsizei WindowHeight;

    EGLContext eglContext;
    EGLDisplay eglDisplay;
    EGLSurface eglSurface;
} UserData;


extern UserData *userData;

class Vector3i {
public:
    int i = 0, j = 0, k = 0;

    Vector3i operator+(Vector3i x) {
        return {x.i + this->i, x.j + this->j, x.k + this->k};
    }

    Vector3i operator-() {
        return {-i, -j, -k};
    }

    bool operator!=(Vector3i value) {
        return i != value.i || j != value.j || k != value.k;
    }

    bool operator==(Vector3i vector3_i) {
        return i == vector3_i.i && j == vector3_i.j && k == vector3_i.k;
    }
};

class Vector3f {
public:
    GLfloat x, y, z;

    Vector3f() : x(0), y(0), z(0) {
    }

    Vector3f(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {
    }

    Vector3f(Vector3i v): x(v.i), y(v.j), z(v.k) {
    }

    Vector3f cross(Vector3f b) {
        return {y * b.z - z * b.y, -(x * b.z - z * b.x), x * b.y - y * b.x};
    }

    Vector3f operator-() {
        return {-x, -y, -z};
    }

    Vector3f operator-(Vector3f v) const {
        return {this->x - v.x, this->y - v.y, this->z - v.z};
    }

    Vector3f operator+(Vector3f v) const {
        return {this->x + v.x, this->y + v.y, this->z + v.z};
    }

    Vector3f operator*(GLfloat v) const {
        return {this->x * v, this->y * v, this->z * v};
    }

    Vector3f operator/(GLfloat v) const {
        return {this->x / v, this->y / v, this->z / v};
    }

    double dot(Vector3f v) const {
        return this->x * v.x + this->y * v.y + this->z * v.z;
    }

    double length() {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    Vector3f normalize() {
        GLfloat length = this->length();
        this->x /= length;
        this->y /= length;
        this->z /= length;
        return {this->x, this->y, this->z};
    }
};

class Vector4f {
public:
    GLfloat x, y, z, w;

    Vector4f() : x(0), y(0), z(0), w(0) {
    }

    Vector4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w) : x(x), y(y), z(z), w(w) {
    }

    Vector4f(Vector3f v, GLfloat w): x(v.x), y(v.y), z(v.z), w(w) {
    }
};


class Matrix4x4 {
public:
    double m[4][4];

    Matrix4x4() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] = 0;
            }
        }

        m[0][0] = 1;
        m[1][1] = 1;
        m[2][2] = 1;
        m[3][3] = 1;
    }

    Vector3f operator*(Vector3f v) const {
        double x = v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2];
        double y = v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2];
        double z = v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2];
        // double w = v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2] + m[3][3];
        double w = 1.0;
        return Vector3f(x / w, y / w, z / w);
    }

    Vector4f operator*(Vector4f v) const {
        double x = v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + v.w * m[0][3];
        double y = v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + v.w * m[1][3];
        double z = v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + v.w * m[2][3];
        double w = v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2] + v.w * m[3][3];
        return Vector4f(x, y, z, w);
    }

    Matrix4x4 operator*(Matrix4x4 m2) const {
        Matrix4x4 res;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                res.m[i][j] = 0;
                for (int k = 0; k < 4; k++) {
                    res.m[i][j] += m[i][k] * m2.m[k][j];
                }
            }
        }

        return res;
    }
};

Matrix4x4 scale(double x, double y, double z);

Matrix4x4 roll(double angle);

Matrix4x4 pitch(double angle);

Matrix4x4 yaw(double angle);

Matrix4x4 translate(double x, double y, double z);

std::vector<GLfloat> flatten(Matrix4x4 m);

Matrix4x4 computePerspectiveMatrix(float fovInRads, float aspectRatio, float near, float far);

Matrix4x4 inverse(Matrix4x4 m);

Matrix4x4 lookAt(Vector3f eye, Vector3f center, Vector3f up);

Vector3f Lerp(Vector3f a, Vector3f b, double t);

#endif //BASE_H
