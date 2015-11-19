#pragma once

#include <GL/gl.h>

class GLPrograms
{
public:
    GLPrograms();
    ~GLPrograms();

    void compilePrograms();
    void cleanupPrograms();

    GLuint getSimpleProg() const;

protected:
    GLint compileShader(GLuint shaderType, const GLchar* shaderSource);
    GLuint compileProgram(
        const GLchar* vertexShaderSource,
        const GLchar* fragmentShaderSource);

    void compileSimpleProgram();

    GLuint m_simpleProg;
};
