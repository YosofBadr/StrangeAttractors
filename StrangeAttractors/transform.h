#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>


class Transform {
public:
	// Our tranform function, allows us to change position, rotation and scale of a vertex
	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f)):
		m_pos(pos),
		m_rot(rot),
		m_scale(scale) {}

	// Calculates the model/world matrix based on the position, rotation and scale.
	inline glm::mat4 GetModel() const {
		glm::mat4 posMatrix = glm::translate(m_pos);
		glm::mat4 rotXMatrix = glm::rotate(m_rot.x, glm::vec3(1, 0, 0));
		glm::mat4 rotYMatrix = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
		glm::mat4 rotZMatrix = glm::rotate(m_rot.z, glm::vec3(0, 0, 1));
		glm::mat4 scaleMatrix = glm::scale(m_scale);

		// Calculates the rotation matrix, the ordering matters, runs from right to left
		glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		// Applies all transformation matrices, order matters
		return posMatrix * rotMatrix * scaleMatrix;
	}

	// Helper functions to get position rotation and scale of a vertex
	inline glm::vec3& GetPos() { return m_pos; }
	inline glm::vec3& GetRot() { return m_rot; }
	inline glm::vec3& GetScale() { return m_scale; }

	// Helper functions to set position rotation and scale of a vertex
	inline void SetPos(const glm::vec3& pos) { m_pos = pos; }
	inline void SetRot(const glm::vec3& rot) { m_rot = rot; }
	inline void SetScale(const glm::vec3& scale) { m_scale = scale; }

protected:
private:
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
};