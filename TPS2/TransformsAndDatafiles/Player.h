#pragma once
#include "DynamicGameObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "PhysicsComponent.h"
#include "Physics.h";

#include "Teabag.h"

#include <vector>

class player : public GameObject
{
public:

	player(){}

	player(Model* modelIn);

	void OnUpdate(float dt) override;
	
	void OnMessage(const std::string m) override;

	std::vector<Teabag*> v_tea;	

	Model* tModel;
	Physics* phy;

	glm::vec3 lastpos;
	glm::vec3 l_lastpos;
	glm::vec3 ll_lastpos;
	glm::vec3 lll_lastpos;
	glm::vec3 llll_lastpos;
	glm::vec3 lllll_lastpos;
	glm::vec3 llllll_lastpos;
	glm::vec3 lllllll_lastpos;
	glm::vec3 llllllll_lastpos;


	int lives = 5;
private:
	float fireDelay = 0.5;
	float delayTimer;
	bool canShoot = true;
	bool alive = true;
	int health = 100;
	float respawnTimer;
	int respawnTime = 5;
};