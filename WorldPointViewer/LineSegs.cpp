#include "LineSegs.h"

#include <algorithm>

#include "Utils.h"

LineSegs::LineSegs(Color color) :
    BufferDrawable(),
    m_color(color)
{
}

LineSegs::~LineSegs()
{
}

void LineSegs::setup(const std::vector<vec3df::Vec3Df>& points) {

    BufferDrawable::setup();

    std::vector<GLfloat> outlineCoords;

    for (auto& p : points) {
        pushCoord4d(p(0), p(1), p(2), outlineCoords);
    }

    const size_t COMPONENTS_PER_VERTEX = 4;
    m_pointCount = outlineCoords.size() / COMPONENTS_PER_VERTEX;

    const GLuint OUTLINE_BUFFER_SIZE = sizeof(GLfloat) * outlineCoords.size();

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, OUTLINE_BUFFER_SIZE, outlineCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void LineSegs::draw(const mat4df::Mat4Df& modelView, const mat4df::Mat4Df& projection) {
    glUseProgram(m_program);

    const GLuint PROG4_MODEL_VIEW_LOC = 0;
    const GLuint PROG4_PROJ_LOC = 1;
    const GLuint PROG4_COLOR_LOC = 2;
    const GLuint PROG4_DIST_FADE_LOC = 3;
    glUniformMatrix4fv(PROG4_MODEL_VIEW_LOC, 1, GL_FALSE, modelView.getBuf());
    glUniformMatrix4fv(PROG4_PROJ_LOC, 1, GL_FALSE, projection.getBuf());
    glUniform4f(PROG4_COLOR_LOC,
        (float)m_color.r / 255.0f,
        (float)m_color.g / 255.0f,
        (float)m_color.b / 255.0f,
        (float)m_color.a / 255.0f);
    glUniform1i(PROG4_DIST_FADE_LOC, 0);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINE_STRIP, 0, m_pointCount);
}
