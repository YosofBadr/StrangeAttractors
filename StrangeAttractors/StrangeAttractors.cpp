#include <iostream>
#include <GL/glew.h>
#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "transform.h"

#define WIDTH 1920
#define HEIGHT 1080

int main(int argc, char* args[])
{
    Display display(WIDTH, HEIGHT, "Strange Attractor");

    Vertex vertices[] = {   
                            Vertex(glm::vec3(-0.01, -0.01, 0.0)), 
                            Vertex(glm::vec3(0, 0.01, 0)),
                            Vertex(glm::vec3(0.01, -0.01, 0)),
                        };

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
    Shader shader("./res/shader");
    Transform transform;

    // While window is running do some drawing
    while (!display.IsClosed()) {
        frameStart = SDL_GetTicks();

        display.Clear(0.0f, 0.15f, 0.3f, 1.0f);

        shader.Bind();
        shader.Update(transform);
        mesh.Draw();

        display.Update();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}

