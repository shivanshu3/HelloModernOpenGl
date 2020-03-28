// HelloModernOpenGl.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include "util.h"
#include "ShaderLoader.h"

GLuint VertexArrayID;
GLuint vertexbuffer;
double lastTime;

void Init()
{
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    static const GLfloat g_vertex_buffer_data[] = {
        -0.5, -0.5, 0,   1, 0, 0,
        -0.5, 0.5, 0,    1, 0.6, 0,
        0.5, 0.5, 0,     1, 1, 0,
        0.5, -0.5, 0,     0, 1, 0,
    };

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer
    (
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        sizeof(GLfloat)*6,                  // stride
        (void*) 0           // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer
    (
        1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        sizeof(GLfloat) * 6,                  // stride
        (void*) (sizeof(GLfloat) * 3)           // array buffer offset
    );

    static const GLuint indices[] = {
        0, 1, 3, 1, 2, 3,
    };

    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLuint programID = LoadShaders("hello.vertex", "hello.fragment");
    if (programID == 0)
        Crash();
    glUseProgram(programID);

    lastTime = glfwGetTime();
}

void RenderLoop()
{
    glDrawElements
    (
        GL_TRIANGLES,      // mode
        6,    // count
        GL_UNSIGNED_INT,   // type
        (void*) 0           // element array buffer offset
    );

    auto timeNow = glfwGetTime();
    auto renderTime = timeNow - lastTime;
    auto fps = 1.0 / renderTime;
    lastTime = timeNow;

    // std::cout << fps << std::endl;
}

int main()
{
    std::cout << "Hello World!\n";

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (GLEW_OK != glewInit())
        return -1;

    glfwSwapInterval(1);
    EnableOpenGlDebugCallback();
    PrintOpenGlVersionInfo();

    Init();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderLoop();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
