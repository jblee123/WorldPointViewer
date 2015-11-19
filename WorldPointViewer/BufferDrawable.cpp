#include "BufferDrawable.h"


BufferDrawable::BufferDrawable() :
    m_program(0),
    m_vao(0),
    m_vbo(0)
{
}

BufferDrawable::~BufferDrawable()
{
}

void BufferDrawable::setProgram(GLuint program) {
    m_program = program;
}

void BufferDrawable::setup() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
}

void BufferDrawable::cleanup() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void BufferDrawable::pushCoord4d(float x, float y, float z, std::vector<GLfloat>& coords) {
    coords.push_back(x);
    coords.push_back(y);
    coords.push_back(z);
    coords.push_back(1);
}

void BufferDrawable::pushCoord4d(float x, float y, std::vector<GLfloat>& coords) {
    pushCoord4d(x, y, 0, coords);
}

void BufferDrawable::pushCoord3d(float x, float y, float z, std::vector<GLfloat>& coords) {
    coords.push_back(x);
    coords.push_back(y);
    coords.push_back(z);
}

void BufferDrawable::pushCoord3d(float x, float y, std::vector<GLfloat>& coords) {
    pushCoord3d(x, y, 0, coords);
}

void BufferDrawable::pushCoord2d(float x, float y, std::vector<GLfloat>& coords) {
    coords.push_back(x);
    coords.push_back(y);
}
