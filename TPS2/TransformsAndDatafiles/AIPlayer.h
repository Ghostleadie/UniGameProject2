#pragma once

#include "GameObject.h"
//#include "DynamicGameObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "Teabag.h"
#include "PhysicsComponent.h"
//#include "ParticleComponent.h"

class AIPlayer : public GameObject {
public:
	AIPlayer()
	{

	}
	Model* tModel;
	AIPlayer(Model* modelIn, glm::vec3 pos);
	void OnUpdate(float dt) override;
	void OnMessage(const std::string m) override;
	bool alive = true;
	vector<Teabag*> v_tea;
private:

	float respawnTimer;
	float respawnTime = 5;


	//float health = 100;
	void destroyAI();
	void spawnAI(glm::vec3 pos);
	glm::vec3 pos;
	glm::quat orient;
};