#include <GLFW/glfw3.h>
//#include "conv.h"
#include <vector>
#include <iostream>

static void draw_scene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(1, 0.0f, 0.0f, -1.0f);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glEnd();
    glFlush();
}

int main()
{
    int width = 100;
    int height = 100;
    if (::glfwInit() != GL_FALSE)
    {
        std::cout << "Failed open" << std::endl;
        return false;
    }
    ::glfwWindowHint(GLFW_VISIBLE, 0);
    GLFWwindow *win = ::glfwCreateWindow(width, height, "tmp", NULL, NULL);
    draw_scene();
    std::vector<std::uint8_t> data(width * height * 4);
    // glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, (char *)&data[0]);
    //convert(width, height, "test2.bmp", (char *)&data[0]);
}
