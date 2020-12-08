#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

//class Vertex {
//public:
//	Vertex(const glm::vec3& pos) {
//		this->pos = pos;
//	}
//
//protected:
//private:
//	glm::vec3 pos;
//};

class Mesh
{
public:
	// Mesh takes in the list of vertices
	Mesh(glm::vec3* vertices, unsigned int numVertices);

	void Draw(glm::vec3* vertices, unsigned int numVertices);

	virtual ~Mesh();

protected:
private:
	Mesh(const Mesh& other) {}
	void operator=(const Mesh& other) {}

	enum {
		POSITION_VB,

		NUM_BUFFERS
	};

	// Refers to mesh data on the GPU
	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount;
};

