#pragma once

#include <math.h>
#include "Vec3Df.h"

namespace vec4df {

typedef vec::Vector<float, 4> Vec4Df;

inline Vec4Df create(const vec3df::Vec3Df& vec3d, float w) {
    Vec4Df vec;
    vec(0) = vec3d(0);
    vec(1) = vec3d(1);
    vec(2) = vec3d(2);
    vec(3) = w;
    return vec;
}

inline Vec4Df create(const vec3df::Vec3Df& vec3d) {
    return create(vec3d, 0);
}

inline Vec4Df create(float x, float y, float z, float w) {
    Vec4Df vec;
    vec(0) = x;
    vec(1) = y;
    vec(2) = z;
    vec(3) = w;
    return vec;
}

inline Vec4Df create(float x, float y, float z) {
    return create(x, y, z, 0);
}

inline Vec4Df create(float x, float y) {
    return create(x, y, 0, 0);
}

inline Vec4Df create(float x) {
    return create(x, 0, 0, 0);
}

inline Vec4Df create() {
    return create(0, 0, 0, 0);
}

} // of namespace vec4df
