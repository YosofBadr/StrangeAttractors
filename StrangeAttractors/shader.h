#pragma once

#include <string>
#include <GL/glew.h>
class Shader
{
public:
	Shader(const std::string& fileName);

	// Attaches the shaders we specify to GPU
	void Bind();
	virtual ~Shader();
protected:
private:
	static const unsigned int NUM_SHADERS = 2;
	Shader(const Shader& other) {}
	void operator=(const Shader& other) {}

	// How OpenGL keeps track of the shader program
	GLuint m_program;

	// Array storing our Vertex and Fragment shaders
	GLuint m_shaders[NUM_SHADERS];
};


