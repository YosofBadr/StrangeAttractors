#include <iostream>
#include <GL/glew.h>
#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "transform.h"
#include "particle.h"

#define WIDTH 1920
#define HEIGHT 1080


float myRandom()
//Return random double within range [0,1]
{
    return (rand() / (float)RAND_MAX);
}


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
    glEnable(GL_POINT_SMOOTH);
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    //Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
    Shader shader("./res/shader");
    Transform transform;

    const unsigned int numOfParticles = 10000;
    Particle particles[numOfParticles];

    // Init particles
    for (int i = 0; i < numOfParticles; i++) { 
        float xCoord = myRandom();
        float yCoord = myRandom();
        float zCoord = myRandom();
        particles[i] = Particle(glm::vec3(xCoord, yCoord, zCoord), glm::vec4(1.0, 1.0, 0.0, 1.0));
    }

    // Change in time
    float dt = 0.001f;

    // Constants/World Variables, change the outcome of the system
    float sigma = 10;
    float rho = 28;
    float beta = 8.0 / 3.0;

    float scale = 0.02f;

    // While window is running do some drawing
    while (!display.IsClosed()) {
        frameStart = SDL_GetTicks();

        display.Clear(0.0f, 0.0f, 0.0f, 0.0f);

        for (int i = 0; i < numOfParticles; i++) {
            float x = particles[i].GetPos().x;
            float y = particles[i].GetPos().y;
            float z = particles[i].GetPos().z;

            // Compute change in position based on current position 
            float dx = (sigma * (y - x)) * dt;
            float dy = ((x * (rho - z)) - y) * dt;
            float dz = ((x * y) - (beta * z)) * dt;

            // Update location
            x += dx;
            y += dy;
            z += dz;

            particles[i].SetPos(glm::vec3(x, y, z));
        }

        // Follows path of lerenz attractor 
        glPointSize(5);
        glBegin(GL_POINTS);
            for (int i = 0; i < numOfParticles; i++) {
                // Draw point
                glVertex3f(particles[i].GetPos().x * scale, particles[i].GetPos().y * scale, particles[i].GetPos().z * scale);
            }
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

