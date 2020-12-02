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

    //Vertex vertices[] = {   
    //                        Vertex(glm::vec3(-0.5, -0.5, 0.0)), 
    //                        Vertex(glm::vec3(0, 0.5, 0)),
    //                        Vertex(glm::vec3(0.5, -0.5, 0)),
    //                    };

    //Vertex vertices[] = {
//                    Vertex(glm::vec3(-0.5, 0.5, 0.0)),
//                    Vertex(glm::vec3(-0.25, -0.5, 0)),
//                    Vertex(glm::vec3(-0.75, -0.5, 0)),
//};
    const int FPS = 30;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    //Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
    Shader shader("./res/shader");
    Transform transform;

    // Change in time
    float dt = 0.005f;

    // Constants/World Variables, change the outcome of the system
    float sigma = 10;
    float rho = 28;
    float beta = 8.0 / 3.0;

    float x = 1;
    float y = 1;
    float z = 1;

    float scale = 0.02f;

    // While window is running do some drawing
    while (!display.IsClosed()) {
        frameStart = SDL_GetTicks();

        display.Clear(0.0f, 0.0f, 0.0f, 0.0f);

        // Follows path of lerenz attractor 
        glPointSize(5);
        glBegin(GL_POINTS);
            // Compute change in position based on current position 
            float dx = (sigma * (y - x)) * dt;
            float dy = ((x * (rho - z)) - y) * dt;
            float dz = ((x * y) - (beta * z)) * dt;

            // Update location
            x += dx;
            y += dy;
            z += dz;

            // Draw point
            glVertex3f(x * scale, y * scale, z * scale);
        glEnd();

        shader.Bind();
        shader.Update(transform);
        //mesh.Draw();

        display.Update();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}

