#include "DynamicGameObject.h"

void DynamicGameObject::OnUpdate(float dt) {
	TransformComponent* transform = getComponent<TransformComponent>();
	glm::vec3 pos = transform->m_position;
	float scaleTimer = 0;
	switch (objectType)
	{
	case rotating:
	{
		if (toggleX)
		{
			transform->pitch(speed.x * dt);
			transform->yaw(speed.y * dt);
			transform->roll(speed.z * dt);
		}
		else {
			transform->pitch(-speed.x * dt);
			transform->yaw(-speed.y * dt);
			transform->roll(-speed.z * dt);
		}
		break;
	}
	case translating:
	{
		if (toggleX)
		{
			pos.x += speed.x * dt;
			if (pos.x > limits.second.x)
			{
				toggleX = false;
			}
		}
		else {
			pos.x -= speed.x * dt;
			if (pos.x < limits.first.x)
			{
				toggleX = true;
			}
		}

		if (toggleY)
		{
			pos.y += speed.y * dt;
			if (pos.y > limits.second.y)
			{
				toggleY = false;
			}
		}
		else {
			pos.y -= speed.y * dt;
			if (pos.y < limits.first.y)
			{
				toggleY = true;
			}
		}

		if (toggleZ)
		{
			pos.z += speed.z * dt;
			if (pos.z > limits.second.z)
			{
				toggleZ = false;
			}
		}
		else {
			pos.z -= speed.z * dt;
			if (pos.z < limits.first.z)
			{
				toggleZ = true;
			}
		}

		getComponent<TransformComponent>()->m_position = pos;
		break;
	}
	case scaling:
	{
		if (toggleX)
		{
			transform->scaleUp(glm::vec3(speed.x * dt,0,0));
			if (transform->m_scale.x > limits.second.x ) {
				toggleX = false;
			}
		}
		else {
			transform->scaleDown(glm::vec3(speed.x * dt, 0, 0));
			if (transform->m_scale.x < limits.first.x) {
				toggleX = true;
			}
		}
		if (toggleY)
		{
			transform->scaleUp(glm::vec3(0, speed.y * dt, 0));
			if (transform->m_scale.y > limits.second.y) {
				toggleY = false;
			}
		}
		else {
			transform->scaleDown(glm::vec3(0, speed.y * dt, 0));
			if (transform->m_scale.y < limits.first.y) {
				toggleY = true;
			}
		}
		if (toggleZ)
		{
			transform->scaleUp(glm::vec3(0,0, speed.z * dt));
			if (transform->m_scale.z > limits.second.z) {
				toggleZ = false;
			}

		}
		else {
			transform->scaleDown(glm::vec3(0, 0, speed.z * dt));
			if (transform->m_scale.z < limits.first.z) {
				toggleZ = true;
			}
		}
		break;
	}
	default:
		break;
	}
}
