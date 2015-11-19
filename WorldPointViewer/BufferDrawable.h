#pragma once

#include <vector>

#include <GL/glew.h>

class BufferDrawable
{
public:
    BufferDrawable();
    virtual ~BufferDrawable();

    virtual void setProgram(GLuint program);
    
    virtual void setup();
    virtual void cleanup();

protected:
    void pushCoord4d(float x, float y, float z, std::vector<GLfloat>& coords);
    void pushCoord4d(float x, float y, std::vector<GLfloat>& coords);
    void pushCoord3d(float x, float y, float z, std::vector<GLfloat>& coords);
    void pushCoord3d(float x, float y, std::vector<GLfloat>& coords);
    void pushCoord2d(float x, float y, std::vector<GLfloat>& coords);

    GLuint m_program;
    GLuint m_vao;
    GLuint m_vbo;
};
