#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public: 
	// Camera position, field of view, min and max render distances
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar) {
		m_perspective = glm::perspective(fov, aspect, zNear, zFar);
		m_position = pos;
		m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
		m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	// Get projection matrix
	inline glm::mat4 GetViewProjection() const {
		return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
	}

	inline glm::vec3& GetPos() { return m_position; }
	inline glm::vec3& GetForward() { return m_forward; }
	inline glm::vec3& GetUp() { return m_up; }

	inline void setPosition(const glm::vec3& pos) { m_position = pos; }
	inline void setForward(const glm::vec3& forward) { m_forward = forward; }
	inline void setUp(const glm::vec3& up) { m_up = up; }

protected:
private:
	// Perspective matrix
	glm::mat4 m_perspective;
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
};

