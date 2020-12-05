#pragma once

#include <string>
#include <GL/glew.h>
#include "transform.h"
#include "camera.h"

class Shader
{
public:
	Shader(const std::string& fileName);

	// Attaches the shaders we specify to GPU
	void Bind();

	// Updates all the uniforms
	void Update(const Transform& transform, const Camera& camera);

	virtual ~Shader();
protected:
private:
	static const unsigned int NUM_SHADERS = 2;
	Shader(const Shader& other) {}
	void operator=(const Shader& other) {}

	// Enumeration which holds the uniforms we use in our shader
	enum {
		TRANSFORM_U,

		NUM_UNIFORMS
	};
	// How OpenGL keeps track of the shader program
	GLuint m_program;

	// Array storing our Vertex and Fragment shaders
	GLuint m_shaders[NUM_SHADERS];

	// Array storinng our uniforms
	GLuint m_uniforms[NUM_UNIFORMS];
};


