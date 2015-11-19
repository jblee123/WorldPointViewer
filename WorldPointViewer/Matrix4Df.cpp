#include "Matrix4Df.h"
#include "Utils.h"

namespace mat4df {

Mat4Df create(
    float f00, float f10, float f20, float f30,
    float f01, float f11, float f21, float f31,
    float f02, float f12, float f22, float f32,
    float f03, float f13, float f23, float f33) {

    Mat4Df mat;

    mat(0, 0) = f00;
    mat(0, 1) = f01;
    mat(0, 2) = f02;
    mat(0, 3) = f03;

    mat(1, 0) = f10;
    mat(1, 1) = f11;
    mat(1, 2) = f12;
    mat(1, 3) = f13;

    mat(2, 0) = f20;
    mat(2, 1) = f21;
    mat(2, 2) = f22;
    mat(2, 3) = f23;

    mat(3, 0) = f30;
    mat(3, 1) = f31;
    mat(3, 2) = f32;
    mat(3, 3) = f33;

    return mat;
}

vec3df::Vec3Df mul(const Mat4Df& mat, const vec3df::Vec3Df& vec) {
    vec4df::Vec4Df vec4d = vec4df::create(vec(0), vec(1), vec(2), 1);
    vec4d = mul(mat, vec4d);
    return vec3df::create(vec4d(0), vec4d(1), vec4d(2));
}

vec4df::Vec4Df mul(const Mat4Df& mat, const vec4df::Vec4Df& vec) {
    vec4df::Vec4Df result;

    for (int row = 0; row < mat.size(); row++) {
        for (int col = 0; col < mat.size(); col++) {
            result(row) += mat(col, row) * vec(col);
        }
    }

    return result;
}

Mat4Df createRotationAbout(const vec4df::Vec4Df& vec, float theta) {
    return createRotationAbout(vec3df::create(vec(0), vec(1), vec(2)), theta);
}

Mat4Df createRotationAbout(const vec3df::Vec3Df& vec, float theta) {

    float absX = fabsf(vec(0));
    float absY = fabsf(vec(1));
    float absZ = fabsf(vec(2));

    vec3df::Vec3Df n;
    if ((absX < absY) && (absX < absZ)) {
        n = vec3df::create(1, 0, 0);
    }
    else if (absY < absZ) {
        n = vec3df::create(0, 1, 0);
    }
    else {
        n = vec3df::create(0, 0, 1);
    }

    vec3df::Vec3Df a = vec.getUnit();
    vec3df::Vec3Df b = vec3df::cross(a, n).getUnit();
    vec3df::Vec3Df c = vec3df::cross(a, b);

    Mat4Df r1 = mat4df::create(
        a(0), a(1), a(2), 0,
        b(0), b(1), b(2), 0,
        c(0), c(1), c(2), 0,
        0, 0, 0, 1);

    float cosTheta = cos(theta);
    float sinTheta = sin(theta);

    Mat4Df r2 = mat4df::create(
        1, 0, 0, 0,
        0, cosTheta, -sinTheta, 0,
        0, sinTheta, cosTheta, 0,
        0, 0, 0, 1);

    Mat4Df r3 = r1;
    r3.transpose();

    return r3 * r2 * r1;
}

} // of namespace mat4df
