#include "display.h"
#include <GL/glew.h>
#include <iostream>

Display::Display(int width, int height, const std::string& title) {
	// Initialising SDL, currently only using SDL for window, if SDL is needed for other functions other than the window then would be a good idea to move to main.
	SDL_Init(SDL_INIT_EVERYTHING);

	// We are using OpenGL to draw on the window (not SDL) so we can set extra parameters that are not in SDL_CreateWindow
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Creating the window 
	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	// Setting up context
	m_glContext = SDL_GL_CreateContext(m_window);

	// Initialise glew and check for errors. Glew searches for OpenGL functions supported.
	GLenum status = glewInit();
	if (status != GLEW_OK) {
		std::cerr << "Glew failed to initialise" << std::endl;
	}

	m_isClosed = false;
}

// Functions run when we close the application. Deconsturcts and deletes from memory.
Display::~Display() {
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

// Helper function to check if window is closed 
bool Display::IsClosed() {
	return m_isClosed;
}

// Allows us to work with double buffers by swapping them.
void Display::Update() {
	SDL_GL_SwapWindow(m_window);
}

// Clear the display by setting color and setting every pixel to that color
void Display::Clear(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}