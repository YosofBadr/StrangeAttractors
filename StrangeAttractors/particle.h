#pragma once

#include <glm/glm.hpp>

class Particle {
public:
	float random()
		//Return random double within range [0,1]
	{
		return (rand() / (float)RAND_MAX);
	}

	Particle() {
		float xCoord = random() * 10.0;
		float yCoord = random() * 10.0;
		float zCoord = random() * 10.0;

		this->m_pos = glm::vec3(xCoord, yCoord, zCoord);
		this->m_prevPos = glm::vec3(0.0, 0.0, 0.0);
		this->m_color = glm::vec4(1.0, 1.0, 0.0, 1.0);
		this->m_lifeSpan = 1.0f;
	}
	Particle(glm::vec3 givenPos, glm::vec4 giveColor) {
		this->m_prevPos = givenPos;
		this->m_pos = givenPos;
		this->m_color = giveColor;
		this->m_lifeSpan = 1.0f;
	}

	// Helper functions to get position rotation and scale of a vertex
	inline glm::vec3& GetPrevPos() { return m_prevPos; }
	inline glm::vec3& GetPos() { return m_pos; }
	inline glm::vec4& GetColor() { return m_color; }
	inline float& GetLifeSpan() { return m_lifeSpan; }

	// Helper functions to set position rotation and scale of a vertex
	inline void SetPos(const glm::vec3& pos) { m_pos = pos; }
	inline void SetPrevPos(const glm::vec3& pos) { m_prevPos = pos; }
	inline void SetColor(const glm::vec4& color) { m_color = color; }
	inline void SetLifeSpan(const float lifeSpan) { m_lifeSpan = lifeSpan; }

protected:
private:
	glm::vec3 m_pos;
	glm::vec3 m_prevPos;
	glm::vec4 m_color;
	float m_lifeSpan;
};

