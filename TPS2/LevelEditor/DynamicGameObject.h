#pragma once
#include "GameObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

class DynamicGameObject : public GameObject
{
public:
	void OnUpdate(float dt) override;
	void OnMessage(const std::string m) override {}

	enum type {
		rotating,
		translating,
		scaling
	};

	type objectType;
	
	glm::vec3 speed;
	std::pair < glm::vec3, glm::vec3> limits;
	bool toggleX = true;
	bool toggleY = true;
	bool toggleZ = true;


	DynamicGameObject() {}

	

};