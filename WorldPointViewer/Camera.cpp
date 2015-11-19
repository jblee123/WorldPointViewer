#include <cstdio>

#include "Camera.h"
#include "Matrix4Df.h"

Camera::Camera() :
    Camera(vec3df::create(0, 0, 0), 0, 0) {
}

Camera::Camera(const vec3df::Vec3Df& position) :
Camera(position, 0, 0) {
}

Camera::Camera(const vec3df::Vec3Df& position, float rotation, float elevation) :
    Camera(position, rotation, elevation, 0) {
}

Camera::Camera(const vec3df::Vec3Df& position, float rotation, float elevation, float twist) :
    m_position(position) {
    //m_fwd(vec3df::create(0, 0, -1)),
    //m_side(vec3df::create(1, 0, 0)),
    //m_up(vec3df::create(0, 1, 0)) {

    //vec3df::rotateX(m_fwd, elevation);
    //vec3df::rotateX(m_up, elevation);

    //vec3df::rotateY(m_fwd, rotation);
    //vec3df::rotateY(m_side, rotation);
    //vec3df::rotateY(m_up, rotation);

    //mat4df::Mat4Df rotationMatrix = mat4df::createRotationAbout(m_fwd, twist);
    //applyRotation(rotationMatrix);
}

Camera::~Camera() {
}

vec3df::Vec3Df Camera::getPosition() const {
    return m_position;
}

void Camera::setPosition(const vec3df::Vec3Df& pos) {
    m_position = pos;
}

vec3df::Vec3Df Camera::getTarget() const {
    //return m_position + m_fwd;
    return vec3df::create(0, 0, 0);
}

vec3df::Vec3Df Camera::getFwd() const {
    //return m_fwd;
    return (m_position * -1).getUnit();
}

vec3df::Vec3Df Camera::getUp() const {
    //return m_up;
    auto fwd = getFwd();
    auto side = getSide();
    auto up = vec3df::cross(side, fwd);
    return up;
}

vec3df::Vec3Df Camera::getSide() const {
    //return m_side;
    auto fwd = getFwd();
    auto up = vec3df::create(0, 0, 1);
    auto side = vec3df::cross(fwd, up);
    return side;
}

//void Camera::moveForward(float delta) {
//    moveBy(m_fwd * delta);
//}
//
//void Camera::moveSide(float delta) {
//    moveBy(m_side * delta);
//}
//
//void Camera::moveUp(float delta) {
//    moveBy(m_up * delta);
//}
//
//void Camera::moveBy(const vec3df::Vec3Df& delta) {
//    m_position = m_position + delta;
//}

//void Camera::printVecs() {
//    printf("\n");
//    printf(" fwd: %.04f, %.04f, %.04f (len: %.04f)\n", m_fwd(0), m_fwd(1), m_fwd(2), m_fwd.length());
//    printf("side: %.04f, %.04f, %.04f (len: %.04f)\n", m_side(0), m_side(1), m_side(2), m_side.length());
//    printf("  up: %.04f, %.04f, %.04f (len: %.04f)\n", m_up(0), m_up(1), m_up(2), m_up.length());
//}
//
//void Camera::spinAroundFwd(float theta) {
//    printf("spinning around fwd\n");
//    printf("start:\n");
//    printVecs();
//    spinAroundVec(m_fwd * -1, theta);
//    printf("result:\n");
//    printVecs();
//}
//
//void Camera::spinAroundSide(float theta) {
//    printf("spinning around side\n");
//    printf("start:\n");
//    printVecs();
//    spinAroundVec(m_side, theta);
//    printf("result:\n");
//    printVecs();
//}
//
//void Camera::spinAroundUp(float theta) {
//    printf("spinning around up\n");
//    printf("start:\n");
//    printVecs();
//    spinAroundVec(m_up, theta);
//    printf("result:\n");
//    printVecs();
//}
//
//void Camera::spinAroundVec(const vec3df::Vec3Df& axis, float theta) {
//    mat4df::Mat4Df rotationMatrix = mat4df::createRotationAbout(axis, theta);
//    applyRotation(rotationMatrix);
//}
//
//void Camera::applyRotation(const mat4df::Mat4Df& rotationMatrix) {
//    m_fwd = mat4df::mul(rotationMatrix, m_fwd);
//    m_side = mat4df::mul(rotationMatrix, m_side);
//    m_up = mat4df::mul(rotationMatrix, m_up);
//}
