#include <cstdlib>
#include <string>

#include <GL/glew.h>

#include "GLPrograms.h"

GLPrograms::GLPrograms() :
    m_simpleProg(0) {
}

GLPrograms::~GLPrograms() {
}

void GLPrograms::compilePrograms() {
    compileSimpleProgram();
}

void GLPrograms::cleanupPrograms() {
    auto cleanupProgram = [](GLuint& prog) {
        if (prog) {
            glDeleteProgram(prog);
            prog = 0;
        }
    };

    cleanupProgram(m_simpleProg);
}

GLuint GLPrograms::getSimpleProg() const {
    return m_simpleProg;
}

GLint GLPrograms::compileShader(GLuint shaderType, const GLchar* shaderSource) {
    const GLchar* SHADER_SOURCE[] = { shaderSource };

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, SHADER_SOURCE, nullptr);
    glCompileShader(shader);

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus) {
        printf("%s shader failed to compile",
            (shaderType == GL_VERTEX_SHADER) ? "vertex" : "fragment");

        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        GLchar* infoLog = new GLchar[logLength + 1];
        glGetShaderInfoLog(shader, logLength, nullptr, infoLog);
        printf("log info:\n%s", infoLog);
        delete[] infoLog;
    }

    return shader;
}

GLuint GLPrograms::compileProgram(
    const GLchar* vertexShaderSource,
    const GLchar* fragmentShaderSource) {

    const GLchar* VERTEX_SHADER_SOURCE[] = { vertexShaderSource };
    const GLchar* FRAGMENT_SHADER_SOURCE[] = { fragmentShaderSource };

    // vertex shader
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    // Create program, attach shaders to it, and link it
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vertexShader);
    glAttachShader(prog, fragmentShader);
    glLinkProgram(prog);

    // Delete the shaders as the program has them now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return prog;
}

void GLPrograms::compileSimpleProgram() {
    const GLchar* VERTEX_SHADER_SOURCE =
        "#version 410 core                                              \n"
        "#extension GL_ARB_explicit_uniform_location : require          \n"
        "                                                               \n"
        "layout (location = 0) in vec4 vertex_pos;                      \n"
        "                                                               \n"
        "layout (location = 0) uniform mat4 mv_matrix;                  \n"
        "layout (location = 1) uniform mat4 proj_matrix;                \n"
        "layout (location = 2) uniform vec4 color;                      \n"
        "layout (location = 3) uniform int dist_fade;                   \n"
        "                                                               \n"
        "out vec4 vs_color;                                             \n"
        "                                                               \n"
        "void main(void) {                                              \n"
        "    gl_Position =                                              \n"
        "        proj_matrix *                                          \n"
        "        mv_matrix *                                            \n"
        "        vertex_pos;                                            \n"
        "    vs_color = color;                                          \n"
        "    if (dist_fade != 0) {                                      \n"
        "        vs_color.w *= clamp(                                   \n"
        "            (6378137 * 0.5) / length(gl_Position), 0.0, 1.0);  \n"
        "    }                                                          \n"
        "}                                                              \n";

    const GLchar* FRAGMENT_SHADER_SOURCE =
        "#version 410 core      \n"
        "                       \n"
        "in vec4 vs_color;      \n"
        "out vec4 color;        \n"
        "void main(void) {      \n"
        "    color = vs_color;  \n"
        "}                      \n";

    m_simpleProg = compileProgram(
        VERTEX_SHADER_SOURCE,
        FRAGMENT_SHADER_SOURCE);
}
