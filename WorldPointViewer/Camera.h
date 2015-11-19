#pragma once

#include "Vec3Df.h"
#include "Vec4Df.h"
#include "Matrix4Df.h"

class Camera
{
public:
    Camera();
    Camera(const vec3df::Vec3Df& position);
    Camera(const vec3df::Vec3Df& position, float rotation, float elevation);
    Camera(const vec3df::Vec3Df& position, float rotation, float elevation, float twist);
    ~Camera();

    vec3df::Vec3Df getPosition() const;
    void setPosition(const vec3df::Vec3Df& pos);
    vec3df::Vec3Df getTarget() const;
    vec3df::Vec3Df getFwd() const;
    vec3df::Vec3Df getUp() const;
    vec3df::Vec3Df getSide() const;

    //void moveForward(float delta);
    //void moveSide(float delta);
    //void moveUp(float delta);
    //void moveBy(const vec3df::Vec3Df& delta);

    //void spinAroundFwd(float theta);
    //void spinAroundSide(float theta);
    //void spinAroundUp(float theta);

protected:
    vec3df::Vec3Df m_position;
    //vec3df::Vec3Df m_fwd;
    //vec3df::Vec3Df m_side;
    //vec3df::Vec3Df m_up;

    //void spinAroundVec(const vec3df::Vec3Df& delta, float theta);
    //void applyRotation(const mat4df::Mat4Df& rotationMatrix);

    //void printVecs();
};
