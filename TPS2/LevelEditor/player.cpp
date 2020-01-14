#include "Player.h"

player::player(Model* modelIn) {
	addComponent(new ModelComponent(modelIn));
	addComponent(new TransformComponent(glm::vec3(0, 10, 50),glm::quat(1, 0, 0, 0),glm::vec3(0.5,0.5,0.5)));
	addComponent(new CameraComponent(glm::vec3(0, 0, 0), glm::quat(1, 0, 0, 0)));
	getComponent<CameraComponent>()->firstPerson = true;
}

void player::OnUpdate(float dt) {

	if (getComponent<CameraComponent>()->firstPerson)
	{
		TransformComponent* transform = getComponent<TransformComponent>();

		getComponent<CameraComponent>()->m_position = -(transform->m_position + (glm::vec3(0, 0, -1) * glm::inverse(transform->m_orientation)));

		getComponent<CameraComponent>()->m_orientation = glm::inverse(transform->m_orientation);

		//glm::vec3 pos = getComponent<CameraComponent>()->m_position;
		//glm::quat orient = getComponent<CameraComponent>()->m_orientation;
		//pos += glm::vec3(1, 0, 0) * orient;
		//getComponent<TransformComponent>()->m_position = -pos;
		//getComponent<TransformComponent>()->m_orientation = glm::inverse(-orient);
	}
	else {
		TransformComponent* transform = getComponent<TransformComponent>();

		glm::quat inverseOrientation = inverse(transform->m_orientation);

		getComponent<CameraComponent>()->m_position = -1.0f * (transform->m_position + glm::vec3(0, 1, 7.5) * inverseOrientation);

		getComponent<CameraComponent>()->m_orientation = inverseOrientation;

		//glm::vec3 pos = getComponent<TransformComponent>()->m_position;
		//glm::quat orient = getComponent<CameraComponent>()->m_orientation;
		//pos -= glm::vec3(0, -0.5, -5) * orient;
		//getComponent<CameraComponent>()->m_position = -pos;
		//getComponent<TransformComponent>()->m_orientation = glm::inverse(-orient);
	}
}

void player::OnMessage(const std::string m) {
	if (m == "reset")
	{
		getComponent<CameraComponent>()->m_position = glm::vec3(0, 1.9, 0);
		getComponent<CameraComponent>()->m_orientation = glm::quat(1, 0, 0, 0);

		getComponent<TransformComponent>()->m_position = glm::vec3(0, 1.9, 0);
		getComponent<TransformComponent>()->m_orientation = glm::quat(1, 0, 0, 0);
	}

	if (m == "Switch")
	{
		if (getComponent<CameraComponent>()->firstPerson)
		{
			getComponent<CameraComponent>()->firstPerson = false;
		}
		else {
			getComponent<CameraComponent>()->firstPerson = true;
		}
		
	}
}