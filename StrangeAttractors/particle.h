#pragma once

#include <glm/glm.hpp>

class Particle {
public:
	Particle() {
		this->m_pos = glm::vec3(0.0, 0.0, 0.0);
		this->m_color = glm::vec4(1.0, 1.0, 0.0, 1.0);
		this->m_lifeSpan = 1.0f;
	}
	Particle(glm::vec3 givenPos, glm::vec4 giveColor) {
		this->m_pos = givenPos;
		this->m_color = giveColor;
		this->m_lifeSpan = 1.0f;
	}

	// Helper functions to get position rotation and scale of a vertex
	inline glm::vec3& GetPos() { return m_pos; }
	inline glm::vec4& GetColor() { return m_color; }
	inline float& GetLifeSpan() { return m_lifeSpan; }

	// Helper functions to set position rotation and scale of a vertex
	inline void SetPos(const glm::vec3& pos) { m_pos = pos; }
	inline void SetColor(const glm::vec4& color) { m_color = color; }
	inline void SetLifeSpan(const float lifeSpan) { m_lifeSpan = lifeSpan; }

protected:
private:
	glm::vec3 m_pos;
	glm::vec4 m_color;
	float m_lifeSpan;
};

