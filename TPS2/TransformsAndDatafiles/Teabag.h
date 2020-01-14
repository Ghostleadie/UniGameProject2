#pragma once

#include "GameObject.h";
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
class Teabag : public GameObject
{
public:
	Teabag() {
	
	}

	Teabag(Model* modelIn, glm::vec3 pos, glm::quat orient) {
		addComponent(new ModelComponent(modelIn));
		addComponent(new TransformComponent(pos, orient));
		addComponent(new PhysicsComponent(pos,0.5,glm::vec3(0.5,0.5,0.5)));

		unitVector = new glm::vec3(orient * glm::vec3(0,0,1));
	}
	
	void OnUpdate(float dt) override {
		TransformComponent* tComp = getComponent<TransformComponent>();
		tComp->m_position.x -= (0.3 * unitVector->x);
		tComp->m_position.y -= (0.3 * unitVector->y);
		tComp->m_position.z -= (0.3 * unitVector->z);
		
		getComponent<PhysicsComponent>()->OnUpdate(dt, tComp->m_position);

		lifeRemaining -= dt;
		//if (lifeRemaining <= 0)
		//{
		//	//delete this;
		//}
	}
	void OnMessage(const std::string m) override{}

	float lifeRemaining = 5;
	~Teabag() {  }
private:
	glm::vec3* unitVector;

};