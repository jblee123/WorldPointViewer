#pragma once

#include <string>

#include "BufferDrawable.h"
#include "Matrix4Df.h"
#include "Color.h"

class LineSegs :
    public BufferDrawable
{
public:
    LineSegs(Color color);
    virtual ~LineSegs();

    virtual void setup(const std::vector<vec3df::Vec3Df>& points);
    virtual void draw(const mat4df::Mat4Df& modelView, const mat4df::Mat4Df& projection);

protected:
    GLuint m_pointCount;
    Color m_color;
};
