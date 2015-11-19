#include "Projection.h"
#include "Utils.h"

namespace projection {

vec4df::Vec4Df v1, v2a, v2b;

mat4df::Mat4Df createLookAt(
    const vec3df::Vec3Df& eye,
    const vec3df::Vec3Df& target,
    const vec3df::Vec3Df& up) {

    mat4df::Mat4Df rot;
    rot.setIdentity();

    vec3df::Vec3Df zAxis = (eye - target).getUnit();
    vec3df::Vec3Df xAxis = vec3df::cross(up, zAxis).getUnit();
    vec3df::Vec3Df yAxis = vec3df::cross(zAxis, xAxis);

    //rot(0, 0) = side(0);
    //rot(1, 0) = side(1);
    //rot(2, 0) = side(2);
    //rot(3, 0) = 0;

    //rot(0, 1) = upPrime(0);
    //rot(1, 1) = upPrime(1);
    //rot(2, 1) = upPrime(2);
    //rot(3, 1) = 0;

    //rot(0, 2) = -fwd(0);
    //rot(1, 2) = -fwd(1);
    //rot(2, 2) = -fwd(2);
    //rot(3, 2) = 0;

    rot(0, 0) = xAxis(0);
    rot(1, 0) = xAxis(1);
    rot(2, 0) = xAxis(2);
    rot(3, 0) = -(xAxis.dot(eye));;

    rot(0, 1) = yAxis(0);
    rot(1, 1) = yAxis(1);
    rot(2, 1) = yAxis(2);
    rot(3, 1) = -(yAxis.dot(eye));

    rot(0, 2) = zAxis(0);
    rot(1, 2) = zAxis(1);
    rot(2, 2) = zAxis(2);
    rot(3, 2) = -(zAxis.dot(eye));

    //mat4df::Mat4Df mat2;
    //mat2.setIdentity();
    //mat2(3, 0) = -eye(0);
    //mat2(3, 1) = -eye(1);
    //mat2(3, 2) = -eye(2);

    //v1 = mat4df::mul(rot, vec4df::create(6500000, 0, 0, 1));

    //v2a = mat4df::mul(mat2, vec4df::create(6500000, 0, 0, 1));
    v2b = mat4df::mul(rot, vec4df::create(6500000, 0, 0, 1));

    //return rot * mat2;
    return rot;
}

mat4df::Mat4Df createPerspective(
    float fov_x, float width, float height, float near_dist, float far_dist) {

    float aspect_ratio = width / height;
    float z_range = far_dist - near_dist;
    //float z_range = far_dist + near_dist;
    float tan_half_fov = tanf(degToRad(fov_x / 2.0f));
    float reciprocal_tan_half_fov = 1.0f / tan_half_fov;

    mat4df::Mat4Df mat;

    mat(0, 0) = reciprocal_tan_half_fov / aspect_ratio;

    mat(1, 1) = reciprocal_tan_half_fov;

    mat(2, 2) = -(far_dist + near_dist) / z_range;
    mat(2, 3) = -2.0f * far_dist * near_dist / z_range;

    mat(3, 2) = -1.0f;

    auto v2bb = v2b;
    vec4df::Vec4Df v3a = mat4df::mul(mat, v2bb);
    vec4df::Vec4Df v3b = v3a / v3a(3);

    return mat;
}

}
