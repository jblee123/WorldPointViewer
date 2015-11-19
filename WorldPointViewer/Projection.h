#include "Matrix4Df.h"
#include "Vec3Df.h"

namespace projection {

mat4df::Mat4Df createLookAt(
    const vec3df::Vec3Df& eye,
    const vec3df::Vec3Df& target,
    const vec3df::Vec3Df& up);

//mat4df::Mat4Df createPerspective(
//    float left, float top, float right, float bottom, float near_dist, float far_dist);

mat4df::Mat4Df createPerspective(
    float fov_x, float width, float height, float near_dist, float far_dist);

//mat4df::Mat4Df createOrtho(
//    float left, float top, float right, float bottom, float near_dist, float far_dist);

}
