#include <iostream>
#include <GL/glew.h>
#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "transform.h"
#include "particle.h"
#include "camera.h"
#include <vector>

#define WIDTH 1920
#define HEIGHT 1080


float random()
//Return random double within range [0,1]
{
    return (rand() / (float)RAND_MAX);
}

std::vector<Particle> initParticles(unsigned int numOfParticles) {
    std::vector<Particle> particles;

    // Init particles
    for (unsigned int i = 0; i < numOfParticles; i++) {
        particles.push_back(Particle());
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    Shader shader("./res/shader");
    Transform transform;
    Camera camera(glm::vec3(0.0f, 0.0f, 2.0f), 70.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

    // Change in time
    float dt = 0.01f;

    // Constants/World Variables - Lorenz Attractor
    float sigma = 10;
    float rho = 28;
    float beta = 8.0 / 3.0;

    float scale = 0.02f;

    float alpha = 1.4f;
    float a = 0.2f;
    float b = 0.2f;
    float r = 5.7;

    const unsigned int numOfParticles = 1000;
    std::vector<Particle> particles = initParticles(numOfParticles);

    //std::vector<glm::vec3> vertices(numOfParticles);
    //glm::vec3 vertices[] = {
    //                    glm::vec3(-0.5, -0.5, 0.0), 
    //                    glm::vec3(0, 0.5, 0),
    //                    glm::vec3(0.5, -0.5, 0),
    //                };

    glm::vec3* vertices = new glm::vec3[numOfParticles];
    //std::vector<Vertex> vertices;

    //vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.0)));
    //vertices.push_back(Vertex(glm::vec3(0, 0.5, 0)));
    //vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0)));
    Mesh mesh(vertices, numOfParticles);
    //Mesh mesh(particles, sizeof(*particles) / sizeof(particles[0]));

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

        display.Clear(0.0f, 0.0f, 0.0f, 1.0f);

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

            //float dx = (-1 * (y + z)) * dt;
            //float dy = (x + (a * y)) * dt;
            //float dz = (b + z * (x - r)) * dt;

            particles[i].SetPrevPos(glm::vec3(x, y, z));

            // Update location
            x += dx;
            y += dy;
            z += dz;

            particles[i].SetPos(glm::vec3(x, y, z));
        }

         //Follows path of lerenz attractor 
        glPointSize(2);
        glBegin(GL_LINES);
            for (int i = 0; i < numOfParticles; i++) {
                // Draw point
                glColor4f(1.0f, 0.0f, 1.0f, 0.5f);
                glVertex3f(particles[i].GetPrevPos().x * scale, particles[i].GetPrevPos().y * scale, particles[i].GetPrevPos().z * scale);
                glVertex3f(particles[i].GetPos().x * scale, particles[i].GetPos().y * scale, particles[i].GetPos().z * scale);
            }
        glEnd();

        //for (int i = 0; i < numOfParticles; i++) {
        //    vertices[i] = particles[i].GetPos();
        //    std::cerr << "x: " << vertices[i].x << std::endl;
        //    std::cerr << "x part " << particles[i].GetPos().x << std::endl;


        //}

        shader.Bind();
        shader.Update(transform, camera);
        mesh.Draw(vertices, numOfParticles);

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
                   
                case SDLK_RIGHTBRACKET:
                    dt += 0.001f;
                    std::cout << "dt increased: " << dt << std::endl;
                    break;

                case SDLK_LEFTBRACKET:
                    dt -= 0.001f;
                    std::cout << "dt decreased: " << dt << std::endl;
                    break;

                // MOVEMENT
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

