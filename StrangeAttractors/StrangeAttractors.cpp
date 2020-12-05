#include <iostream>
#include <GL/glew.h>
#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "transform.h"
#include "particle.h"
#include "camera.h"

#define WIDTH 1920
#define HEIGHT 1080


float myRandom()
//Return random double within range [0,1]
{
    return (rand() / (float)RAND_MAX);
}

Particle* initParticles(int numOfParticles) {
    Particle particles[1000];
    // Init particles
    for (int i = 0; i < numOfParticles; i++) {
        float xCoord = myRandom() * 5.0;
        float yCoord = myRandom() * 5.0;
        float zCoord = myRandom() * 5.0;
        particles[i] = Particle(glm::vec3(xCoord, yCoord, zCoord), glm::vec4(1.0f, 1.0f, 0.0f, 0.5f));
    }

    return particles;
};

int main(int argc, char* args[])
{
    Display display(WIDTH, HEIGHT, "Strange Attractor");

    //Vertex vertices[] = {   
    //                        Vertex(glm::vec3(-0.5, -0.5, 0.0)), 
    //                        Vertex(glm::vec3(0, 0.5, 0)),
    //                        Vertex(glm::vec3(0.5, -0.5, 0)),
    //                    };

    glEnable(GL_POINT_SMOOTH);
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    // Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
    Shader shader("./res/shader");
    Transform transform;
    Camera camera(glm::vec3(0.0f, 0.0f, 2.0f), 70.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

    const unsigned int numOfParticles = 1000;
    Particle* particles = initParticles(numOfParticles);

    // Change in time
    float dt = 0.01f;
    dt = 0.01f;

    // Constants/World Variables, change the outcome of the system
    float sigma = 10;
    float rho = 28;
    float beta = 8.0 / 3.0;
    float alpha = 1.4f;

    float scale = 0.02f;

    // Camera settings
    const float cameraSpeed = 0.2f;
    const float cameraSensitivity = 0.05f;
    float prevX = WIDTH / 2;
    float prevY = HEIGHT / 2;
    float yaw = -90.0f;
    float pitch = 0.0f;
    bool firstMouse = true;

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

            // Compute change in position based on current position 
            //float dx = ((alpha * x * -1) - (4 * y) - (4 * z) - (y * y)) * dt;
            //float dy = ((alpha * y * -1) - (4 * z) - (4 * x) - (z * z)) * dt;
            //float dz = ((alpha * z * -1) - (4 * x) - (4 * y) - (x * x)) * dt;

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
        shader.Update(transform, camera);
        // mesh.Draw();

        // Event handling
        SDL_Event Event;
        glm::vec3& cameraPos = camera.GetPos();;

        while (SDL_PollEvent(&Event)) {
            switch (Event.type) {
            case SDL_QUIT:
                display.SetState(true);
                break;

            case SDL_KEYDOWN:
                switch (Event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    display.SetState(true); 
                    break;

                case SDLK_SPACE:
                    particles = initParticles(numOfParticles); 
                    break;


                case SDLK_w:
                    cameraPos += cameraSpeed * camera.GetForward();
                    camera.setPosition(cameraPos);
                    break;


                case SDLK_s:
                    cameraPos -= cameraSpeed * camera.GetForward();
                    camera.setPosition(cameraPos);
                    break;


                case SDLK_a:
                    cameraPos -= glm::normalize(glm::cross(camera.GetForward(), camera.GetUp())) * cameraSpeed;
                    camera.setPosition(cameraPos);
                    break;


                case SDLK_d:
                    cameraPos += glm::normalize(glm::cross(camera.GetForward(), camera.GetUp())) * cameraSpeed;
                    camera.setPosition(cameraPos);
                    break;
                }
                break;

            case SDL_MOUSEMOTION:
                float deltaX = Event.motion.xrel;;
                float deltaY = - Event.motion.yrel;

                deltaX *= cameraSensitivity;
                deltaY *= cameraSensitivity;

                yaw += deltaX;
                pitch += deltaY;

                if (pitch > 89.0f)
                    pitch = 89.0f;
                if (pitch < -89.0f)
                    pitch = -89.0f;

                glm::vec3 newForward;
                newForward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                newForward.y = sin(glm::radians(pitch));
                newForward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                camera.setForward(glm::normalize(newForward));
                break;
            }
        }

        display.Update();
        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}

