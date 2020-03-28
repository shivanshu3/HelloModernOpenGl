#include "stdafx.h"
#include "util.h"

typedef void (GLAPIENTRY* DEBUGPROC)
(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
);

void Crash()
{
    *(reinterpret_cast<char*>(0)) = 0;
}

static void GLAPIENTRY OpenGlDebugCallback
(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
)
{
    bool crashProgram = false;

    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        std::cout << "OpenGL Notification:\n";
    }
    else
    {
        std::cout << "OpenGL Warning/Error:\n";
        crashProgram = true;
    }

    std::cout << message << std::endl;

    if (crashProgram)
        Crash();
}

void EnableOpenGlDebugCallback()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(OpenGlDebugCallback, nullptr);
}

void PrintOpenGlVersionInfo()
{
    auto printVersion = [](uint32_t type)
    {
        auto* str = (const char*) glGetString(type);
        if (str)
            std::cout << str << std::endl;
        else
            std::cout << "Error" << std::endl;
    };

    printVersion(GL_VERSION);
    printVersion(GL_VENDOR);
    printVersion(GL_RENDERER);
}
