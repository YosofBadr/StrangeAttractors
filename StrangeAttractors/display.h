#pragma once

#include <string>
#include <SDL2/SDL.h>

class Display
{
public:
	Display(int width, int height, const std::string& title);

	void Update();
	void Clear(float r, float g, float b, float a);
	bool IsClosed();

	inline void SetState(bool state) { m_isClosed = state; }


	virtual ~Display();
protected:
private:
	Display(const Display & other) {}
	void operator=(const Display& other) {}

	// Window pointer so that we can refer to the location of the window.
	SDL_Window* m_window;

	// Creating a context as this is required for OpenGL to draw to the window.
	SDL_GLContext m_glContext;

	// Whether the window is closed or not
	bool m_isClosed;
};

