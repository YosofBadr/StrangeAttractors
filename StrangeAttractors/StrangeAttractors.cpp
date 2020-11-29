#include <iostream>
#include <GL/glew.h>
#include "display.h"
#include "shader.h"

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char* args[])
{
    Display display(WIDTH, HEIGHT, "Strange Attractor");

    Shader shader("./res/shader");

    // While window is running do some drawing
    while (!display.IsClosed()) {
        display.Clear(0.0f, 0.15f, 0.3f, 1.0f);
        shader.Bind();


        display.Update();
    }

    return 0;
}

