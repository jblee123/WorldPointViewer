#pragma once

#include <math.h>
#include "Vector.h"

namespace vec3df {

typedef vec::Vector<float, 3> Vec3Df;

inline Vec3Df create(float x, float y, float z) {
    Vec3Df vec;
    vec(0) = x;
    vec(1) = y;
    vec(2) = z;
    return vec;
}

inline Vec3Df create(float x, float y) {
    return create(x, y, 0);
}

inline Vec3Df create(float x) {
    return create(x, 0, 0);
}

inline Vec3Df create() {
    return create(0, 0, 0);
}

inline Vec3Df& translate(Vec3Df& vec, float x, float y, float z) {
    vec(0) += x;
    vec(1) += y;
    vec(2) += z;
    return vec;
}

inline Vec3Df& rotateX(Vec3Df& vec, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    float y = vec(1);
    float z = vec(2);
    vec(1) = y * c - z * s;
    vec(2) = y * s + z * c;
    return vec;
}

inline Vec3Df& rotateY(Vec3Df& vec, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    float x = vec(0);
    float z = vec(2);
    vec(0) = x * c + z * s;
    vec(2) = -x * s + z * c;
    return vec;
}

inline Vec3Df& rotateZ(Vec3Df& vec, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    float x = vec(0);
    float y = vec(1);
    vec(0) = x * c - y * s;
    vec(1) = x * s + y * c;
    return vec;
}

inline Vec3Df cross(const Vec3Df& vec, const Vec3Df& rhs) {
    Vec3Df result;
    result(0) = vec(1) * rhs(2) - vec(2) * rhs(1);
    result(1) = vec(2) * rhs(0) - vec(0) * rhs(2);
    result(2) = vec(0) * rhs(1) - vec(1) * rhs(0);
    return result;
}


} // of namespace vec3df
