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


float random()
//Return random double within range [0,1]
{
    return (rand() / (float)RAND_MAX);
}

std::vector<Particle> initParticles(unsigned int numOfParticles, float spread) {
    std::vector<Particle> particles;

    // Init particles
    for (unsigned int i = 0; i < numOfParticles; i++) {
        particles.push_back(Particle(spread));
    }

    return particles;
};

//unsigned int lastFreeIndex = 0;
//unsigned int findFreeIndex(std::vector<Particle> particles) {
//    for (unsigned int i = lastFreeIndex; i < particles.size(); i++) {
//        if (particles[i].GetLifeSpan() <= 0.0f) {
//            lastFreeIndex = i;
//            return i;
//        }
//    }
//    // otherwise, do a linear search
//    for (unsigned int i = 0; i < lastFreeIndex; i++) {
//        if (particles[i].GetLifeSpan() <= 0.0f) {
//            lastFreeIndex = i;
//            return i;
//        }
//    }
//    // override first particle if all others are alive
//    lastFreeIndex = 0;
//    return 0;
//}

void printInstructions() {
    std::cout << "KEYBINDS" << std::endl;
    std::cout << "r/R: increase/decrease red channel" << std::endl;
    std::cout << "g/G: increase/decrease green channel" << std::endl;
    std::cout << "b/B: increase/decrease blue channel" << std::endl;
    std::cout << "l/L: increase/decrease lifetime" << std::endl;
    std::cout << "t/T: increase/decrease change in time" << std::endl;
    std::cout << "i/I: increase/decrease sigma" << std::endl;
    std::cout << "o/O: increase/decrease rho" << std::endl;
    std::cout << "p/P: increase/decrease beta" << std::endl;
    std::cout << "c/C: increase/decrease spread" << std::endl;
    std::cout << "1-4: change attractor" << std::endl;

}

int main(int argc, char* args[])
{
    Display display(WIDTH, HEIGHT, "Strange Attractor");
    
    printInstructions();
    //Vertex vertices[] = {   
    //                        Vertex(glm::vec3(-0.5, -0.5, 0.0)), 
    //                        Vertex(glm::vec3(0, 0.5, 0)),
    //                        Vertex(glm::vec3(0.5, -0.5, 0)),
    //                    };

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    Shader shader("./res/shader");
    Transform transform;
    Camera camera(glm::vec3(0.0f, 0.0f, 2.0f), 70.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

    // Change in time
    float dt = 0.005f;

    // Rate of life span decay
    float decay = 0.002f;

    int attractor = 1;

    // Constants/World Variables - Lorenz Attractor
    float sigma = 10;
    float rho = 28;
    float beta = 8.0 / 3.0;

    float red = 0.0f;
    float blue = 0.0f;
    float green = 0.0f;

    float scale = 0.02f;

    float alpha = 1.4f;
    float a = 0.2f;
    float b = 0.2f;
    float r = 5.7;

    const unsigned int numOfParticles = 25000;
    unsigned int numberOfDead = 0;
    float spread = 10.0;
    std::vector<Particle> particles = initParticles(numOfParticles, spread);

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
    //Mesh mesh(vertices, numOfParticles);
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


        //for (int i = 0; i < 2; i++) {
        //    int emptyIndex = findFreeIndex(particles);

        //    particles[emptyIndex] = Particle();

        //    numberOfDead = 0;
        //}

        // Update particles
        for (int i = 0; i < numOfParticles; i++) {
            particles[i].GetLifeSpan() -= decay;
            if (particles[i].GetLifeSpan() > 0) {
                float x = particles[i].GetPos().x;
                float y = particles[i].GetPos().y;
                float z = particles[i].GetPos().z;

                float dx, dy, dz;

                // Compute change in position based on attractor
                switch (attractor) {
                    case 1:
                        dx = (sigma * (y - x)) * dt;
                        dy = ((x * (rho - z)) - y) * dt;
                        dz = ((x * y) - (beta * z)) * dt;
                    break;

                    case 2:
                        dx = ((alpha * x * -1) - (4 * y) - (4 * z) - (y * y)) * dt;
                        dy = ((alpha * y * -1) - (4 * z) - (4 * x) - (z * z)) * dt;
                        dz = ((alpha * z * -1) - (4 * x) - (4 * y) - (x * x)) * dt;
                        break;

                    case 3:
                        dx = (-1 * (y + z)) * dt;
                        dy = (x + (a * y)) * dt;
                        dz = (b + z * (x - r)) * dt;
                        break;

                    case 4:
                        float a = 0.3;
                        float s = 1.0;
                        dx = (x * (4 - y) + a * z) * dt;
                        dy = (-y * (1 - x * x)) * dt;
                        dz = (-x * (1.5 - s * z) - 0.05 * z)* dt;
                        //float a = 40;
                        //float c = 1.833;
                        //float d = 0.16;
                        //float e = 0.65;
                        //float k = 55;
                        //float f = 20;

                        //dx = ((a * (y - x)) + (d*x*z)) * dt;
                        //dy = (k * x + f * y - x * z) * dt;
                        //dz = (c*z + x*y - e*x*x) * dt;
                        break;
                                   
                }

                particles[i].SetPrevPos(glm::vec3(x, y, z));

                // Update location
                x += dx;
                y += dy;
                z += dz;

                particles[i].SetPos(glm::vec3(x, y, z));

                //Follows path of lerenz attractor 
                glBegin(GL_LINES);

                    glColor4f(particles[i].GetColor().x + red, particles[i].GetColor().y + green, particles[i].GetColor().z + blue, particles[i].GetLifeSpan());
                    glVertex3f(particles[i].GetPrevPos().x* scale, particles[i].GetPrevPos().y* scale, particles[i].GetPrevPos().z* scale);
                    glVertex3f(particles[i].GetPos().x* scale, particles[i].GetPos().y* scale, particles[i].GetPos().z* scale);

                glEnd();

            }
            else {
                particles[i] = Particle(spread);
            }
        }

        //glPointSize(5);
        //glBegin(GL_POINTS);
        //for (int i = 0; i < numOfParticles; i++) {
        //    // Draw Particle trail
        //    if (particles[i].GetLifeSpan() > 0.0f) {
        //        glColor4f(1.0f, 0.0f, 1.0f, particles[i].GetLifeSpan() + 0.05f);
        //        glVertex3f(particles[i].GetPrevPos().x * scale, particles[i].GetPrevPos().y * scale, particles[i].GetPrevPos().z * scale);
        //    }
        //}
        //glEnd();

        //Follows path of lerenz attractor 
        //glBegin(GL_LINES);
        //    for (int i = 0; i < numOfParticles; i++) {
        //        // Draw Particle trail
        //        if (particles[i].GetLifeSpan() > 0.0f) {
        //            glColor4f(particles[i].GetColor().x + red, particles[i].GetColor().y + green, particles[i].GetColor().z + blue, particles[i].GetLifeSpan());
        //            //glBegin(GL_POINTS);
        //            //    glVertex3f(particles[i].GetPos().x * scale, particles[i].GetPos().y * scale, particles[i].GetPos().z * scale);
        //            //glEnd();
        //            glVertex3f(particles[i].GetPrevPos().x* scale, particles[i].GetPrevPos().y* scale, particles[i].GetPrevPos().z* scale);
        //            glVertex3f(particles[i].GetPos().x* scale, particles[i].GetPos().y* scale, particles[i].GetPos().z* scale);
        //        }
        //    }
        //glEnd();

        //for (int i = 0; i < numOfParticles; i++) {
        //    vertices[i] = particles[i].GetPos();
        //    std::cerr << "x: " << vertices[i].x << std::endl;
        //    std::cerr << "x part " << particles[i].GetPos().x << std::endl;
        //}

        shader.Bind();
        shader.Update(transform, camera);
        //mesh.Draw(vertices, numOfParticles);

        // Event handling
        SDL_Event Event;
        const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);
        glm::vec3& cameraPos = camera.GetPos();;

        // KEYBINDS
        // ESC - CLOSE
        // SPACE - RESET
        // r/R to increase/decrease red 
        // b/B to increase/decrease blue 
        // g/G to increase/decrease green 
        // l/L to increase/decrease lifetime
        // l/L to increase/decrease lifetime
        // t/T to increase/decrease change in time

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
                    particles = initParticles(numOfParticles, spread);
                    break;
                   
                case SDLK_r:
                    if (keyboard_state_array[SDL_SCANCODE_R] && !(keyboard_state_array[SDL_SCANCODE_LCTRL])) {
                        red -= 0.05f;
                        std::cout << "Red decreased: " << red << std::endl;
                    }
                    else {
                        red += 0.05f;
                        std::cout << "Red increased: " << red << std::endl;
                    }
                    break;

                case SDLK_b:
                    if (keyboard_state_array[SDL_SCANCODE_B] && !(keyboard_state_array[SDL_SCANCODE_LCTRL])) {
                        blue -= 0.05f;
                        std::cout << "Blue decreased: " << blue << std::endl;
                    }
                    else {
                        blue += 0.05f;
                        std::cout << "Blue increased: " << blue << std::endl;
                    }
                    break;

                case SDLK_g:
                    if (keyboard_state_array[SDL_SCANCODE_G] && !(keyboard_state_array[SDL_SCANCODE_LCTRL])) {
                        green -= 0.05f;
                        std::cout << "Green decreased: " << green << std::endl;
                    }
                    else {
                        green += 0.05f;
                        std::cout << "Green increased: " << green << std::endl;
                    }
                    break;

                case SDLK_l:
                    if (keyboard_state_array[SDL_SCANCODE_L] && !(keyboard_state_array[SDL_SCANCODE_LCTRL])) {
                        decay += 0.002f;
                        std::cout << "Lifetime decreased: " << decay << std::endl;

                    }
                    else {
                        decay -= 0.002f;
                        std::cout << "Lifetime increased: " << decay << std::endl;
                    }
                    break;

                case SDLK_t:
                    if (keyboard_state_array[SDL_SCANCODE_T] && !(keyboard_state_array[SDL_SCANCODE_LCTRL])) {
                        dt -= 0.0005f;
                        std::cout << "dt decreased: " << dt << std::endl;
                    }
                    else {
                        dt += 0.0005f;
                        std::cout << "dt increased: " << dt << std::endl;
                    }
                    break;

                case SDLK_i:
                    if (keyboard_state_array[SDL_SCANCODE_I] && !(keyboard_state_array[SDL_SCANCODE_LCTRL])) {
                        sigma -= 0.1f;
                        std::cout << "Sigma decreased: " << sigma << std::endl;
                    }
                    else {
                        sigma += 0.1f;
                        std::cout << "Sigma increased: " << sigma << std::endl;
                    }
                    break;

                case SDLK_o:
                    if (keyboard_state_array[SDL_SCANCODE_O] && !(keyboard_state_array[SDL_SCANCODE_LCTRL])) {
                        rho -= 0.1f;
                        std::cout << "Rho decreased: " << rho << std::endl;
                    }
                    else {
                        rho += 0.1f;
                        std::cout << "Rho increased: " << rho << std::endl;
                    }
                    break;

                case SDLK_p:
                    if (keyboard_state_array[SDL_SCANCODE_P] && !(keyboard_state_array[SDL_SCANCODE_LCTRL])) {
                        beta -= 0.1f;
                        std::cout << "Beta decreased: " << beta << std::endl;
                    }
                    else {
                        beta += 0.1f;
                        std::cout << "Beta increased: " << beta << std::endl;
                    }
                    break;

                case SDLK_c:
                    if (keyboard_state_array[SDL_SCANCODE_C] && !(keyboard_state_array[SDL_SCANCODE_LCTRL])) {
                        spread -= 2.0f;
                        std::cout << "Spread decreased: " << spread << std::endl;
                    }
                    else {
                        spread += 2.0f;
                        std::cout << "Spread increased: " << spread << std::endl;
                    }
                    break;

                // Select Attractor
                case SDLK_1:
                    scale = 0.02f;
                    attractor = 1;
                    dt = 0.005f;
                    particles = initParticles(numOfParticles, spread);
                    break;

                case SDLK_2:
                    scale = 0.02f;
                    attractor = 2;
                    dt = 0.0005f;
                    particles = initParticles(numOfParticles, spread);
                    break;

                case SDLK_3:
                    scale = 0.0005f;
                    attractor = 3;
                    dt = 0.01f;
                    particles = initParticles(numOfParticles, spread);
                    break;

                case SDLK_4:
                    attractor = 4;
                    particles = initParticles(numOfParticles, spread);
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
                if (!keyboard_state_array[SDL_SCANCODE_V]) {
                    float deltaX = Event.motion.xrel;;
                    float deltaY = -Event.motion.yrel;

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
                else {
                    float angleX = Event.motion.xrel;
                    float angleY = Event.motion.yrel;
                    transform.GetRot().x += glm::radians(angleY) * cameraSensitivity;
                    transform.GetRot().y += glm::radians(angleX) * cameraSensitivity;

                }
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

