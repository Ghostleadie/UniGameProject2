#pragma once
#include "Component.h"

class PhysicsComponent : public Component {

public:

	PhysicsComponent() {}

	PhysicsComponent(glm::vec3 pos, float r, glm::vec3 size) {
		radius = r;
		m_size = size;
		m_position = pos;
	}


	glm::vec3 m_size;
	glm::vec3 m_position;
	float radius;
	void OnUpdate(float dt) override {};

	void OnMessage(const std::string m) override {};

	void OnUpdate(float dt, glm::vec3 pos) {
		m_position = pos;
	}
};
