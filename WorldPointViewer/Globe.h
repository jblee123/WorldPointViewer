#pragma once

#include "BufferDrawable.h"
#include "Matrix4Df.h"

class Globe :
    public BufferDrawable
{
public:
    Globe(double resolution);
    virtual ~Globe();

    virtual void setup();
    virtual void draw(const mat4df::Mat4Df& modelView, const mat4df::Mat4Df& projection);

protected:
    GLuint m_pointCount;
};
