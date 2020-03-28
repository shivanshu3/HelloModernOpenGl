// HelloModernOpenGl.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include "util.h"
#include "ShaderLoader.h"

GLint uBlue = -1;
float blueValue = 0;
bool blueValueInc = true;
GLuint elementbuffer1;
GLuint elementbuffer2;
double lastTime;

void Init()
{
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    static const GLfloat g_vertex_buffer_data[] = {
        -0.5, -0.5, 0,   1, 0, 0, 1,
        -0.5, 0.5, 0,    1, 0, 0, 1,
        0.5, -0.5, 0,     1, 0, 0, 1,

        -0.5, -0.5, 0,   0, 1, 0, 0.5,
        0.5, 0.5, 0,     0, 1, 0, 0.5,
        0.5, -0.5, 0,     0, 1, 0, 1.0,
    };

    GLuint vertexbuffer;
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
        sizeof(GLfloat)*7,                  // stride
        (void*) 0           // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer
    (
        1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        4,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        sizeof(GLfloat) * 7,                  // stride
        (void*) (sizeof(GLfloat) * 3)           // array buffer offset
    );

    static const GLuint indices1[] = {
        0, 1, 2, 3, 4, 5
    };

    static const GLuint indices2[] = {
        // 1, 2, 3,
        0, 2, 3,
    };

    glGenBuffers(1, &elementbuffer1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

    glGenBuffers(1, &elementbuffer2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint programID = LoadShaders("hello.vertex", "hello.fragment");
    if (programID == 0)
        Crash();
    glUseProgram(programID);

    uBlue = glGetUniformLocation(programID, "uBlue");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    lastTime = glfwGetTime();
}

void UpdateBlueValue()
{
    constexpr float incSize = 0.1;

    if (blueValue >= 1.0)
    {
        blueValueInc = false;
    }

    if (blueValue <= 0.0)
    {
        blueValueInc = true;
    }

    if (blueValueInc)
    {
        blueValue += incSize;
    }
    else
    {
        blueValue -= incSize;
    }

    if (blueValue > 1.0)
    {
        blueValue = 1.0;
    }

    if (blueValue < 0.0)
    {
        blueValue = 0.0;
    }
}

void RenderLoop()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer1);

    UpdateBlueValue();
    glUniform1f(uBlue, blueValue);

    glDrawElements
    (
        GL_TRIANGLES,      // mode
        6,    // count
        GL_UNSIGNED_INT,   // type
        nullptr           // element array buffer offset
    );

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer2);
    // glUniform3f(colorUniformLocation, 0, 1, 0);
    // 
    // glDrawElements
    // (
    //     GL_TRIANGLES,      // mode
    //     3,    // count
    //     GL_UNSIGNED_INT,   // type
    //     nullptr           // element array buffer offset
    // );

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
