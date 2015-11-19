#pragma once

#include "Matrix.h"
#include "Vec4Df.h"

#pragma once

namespace mat4df {

typedef mat::Matrix<float, 4> Mat4Df;

Mat4Df create(
    float f00, float f01, float f02, float f03,
    float f10, float f11, float f12, float f13,
    float f20, float f21, float f22, float f23,
    float f30, float f31, float f32, float f33);
vec4df::Vec4Df mul(const Mat4Df& mat, const vec4df::Vec4Df& vec);
vec3df::Vec3Df mul(const Mat4Df& mat, const vec3df::Vec3Df& vec);
Mat4Df createRotationAbout(const vec3df::Vec3Df& vec, float theta);
Mat4Df createRotationAbout(const vec4df::Vec4Df& vec, float theta);
Mat4Df createRotationAbout(const vec3df::Vec3Df& vec, float theta, const vec3df::Vec3Df& theVec);

} // of namespace mat4df
