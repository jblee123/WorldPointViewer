#include "Globe.h"

#include <algorithm>

#include "Utils.h"

Globe::Globe(double resolution) :
    BufferDrawable(),
    m_pointCount(0)
{
}

Globe::~Globe()
{
}

void Globe::setup() {

    BufferDrawable::setup();

    std::vector<GLfloat> outlineCoords;

    const float RAD = (float)EARTH_EQUITORIAL_RADIUS;

    auto getPointFor = [&](int lat, int lon) {
        auto p = vec3df::create(1, 0, 0);
        float lat_f = degToRad((float)lat);
        float lon_f = degToRad((float)lon);

        p = vec3df::rotateY(p, lat_f);
        p = vec3df::rotateZ(p, lon_f);
        p *= RAD;

        return p;
    };

    // latitude segs
    for (int lat = -89; lat <= 89; lat++) {
        for (int lon = -180; lon <= 179; lon++) {
            auto p1 = getPointFor(lat, lon);
            auto p2 = getPointFor(lat, lon + 1);

            pushCoord4d(p1(0), p1(1), p1(2), outlineCoords);
            pushCoord4d(p2(0), p2(1), p2(2), outlineCoords);
        }
    }

    // longitude segs
    for (int lat = -90; lat <= 89; lat++) {
        for (int lon = -179; lon <= 180; lon++) {
            auto p1 = getPointFor(lat, lon);
            auto p2 = getPointFor(lat + 1, lon);

            pushCoord4d(p1(0), p1(1), p1(2), outlineCoords);
            pushCoord4d(p2(0), p2(1), p2(2), outlineCoords);
        }
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

void Globe::draw(const mat4df::Mat4Df& modelView, const mat4df::Mat4Df& projection) {
    glUseProgram(m_program);

    const GLuint PROG4_MODEL_VIEW_LOC = 0;
    const GLuint PROG4_PROJ_LOC = 1;
    const GLuint PROG4_COLOR_LOC = 2;
    const GLuint PROG4_DIST_FADE_LOC = 3;
    glUniformMatrix4fv(PROG4_MODEL_VIEW_LOC, 1, GL_FALSE, modelView.getBuf());
    glUniformMatrix4fv(PROG4_PROJ_LOC, 1, GL_FALSE, projection.getBuf());
    glUniform4f(PROG4_COLOR_LOC, 0.3, 0.3, 0.3, 1);
    glUniform1i(PROG4_DIST_FADE_LOC, 1);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, m_pointCount);
}
