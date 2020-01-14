#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>

class TransformComponent : public Component
{
public:
	float rotateSpeed = 0.01f;
	bool isMoving;
	glm::vec3 x_axis = glm::vec3(1, 0, 0);
	glm::vec3 y_axis = glm::vec3(0, 1, 0);
	glm::vec3 z_axis = glm::vec3(0, 0, 1);

	glm::vec3 m_position;
	glm::quat m_orientation;
	glm::vec3 m_scale;

	glm::vec3 lastpos;

	void OnUpdate(float dt) override
	{		
		if (isMoving)
		{
			if (lastpos == m_position)
			{
				isMoving = false;
			}
		}
		lastpos = m_position;
	}

	void OnMessage(const std::string m) override
	{
		if (m == "MoveForward")
		{
			//if(phy->CircleRec)
			translateWithOrient(glm::vec3(0, 0, -0.1));
			isMoving = true;
			std::cout << "ismoving" << isMoving << std::endl;
		}

		if (m == "MoveBack")
		{
			translateWithOrient(glm::vec3(0, 0, 0.1));
			isMoving = true;
		}

		if (m == "MoveLeft")
		{
			translateWithOrient(glm::vec3(-0.1, 0, 0));
			isMoving = true;
		}

		if (m == "MoveRight")
		{
			translateWithOrient(glm::vec3(0.1, 0, 0));
			isMoving = true;
		}

		if (m == "MoveUp")
		{
			translate(glm::vec3(0, -0.1, 0));
		}

		if (m == "MoveDown")
		{
			translate(glm::vec3(0, 0.1, 0));
		}

		if (m == "resetCam")
		{
			m_position = glm::vec3(0, 0, 0);
			m_orientation = glm::quat(1, 0, 0, 0);
		}
		/*if (m == "reset")
		{
			m_position = glm::vec3(0, 1.9, 0);
			m_orientation = glm::quat(1, 0, 0, 0);

			m_position = glm::vec3(0, 1.9, 0);
			m_orientation = glm::quat(1, 0, 0, 0);
		}*/
		/*
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

		if (m == "rangedHit")
		{
			health -= 25;
			std::cout << "\n PLAYER HIT FROM RANGE \n";
		}

		if (m == "meleeHit")
		{
			health -= 50;

		}

		if (m == "fire")
		{
			if (canShoot)
			{

				Teabag* t;

				TransformComponent* tComp = getComponent<TransformComponent>();


				t = new Teabag(tModel, tComp->m_position, tComp->m_orientation);

				v_tea.push_back(t);
				canShoot = false;
				delayTimer = 0;
			}
		}*/

	}

	TransformComponent() : m_position(0), m_orientation(1, 0, 0, 0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos) : m_position(pos), m_orientation(1, 0, 0, 0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos, const glm::quat& orient) : m_position(pos), m_orientation(orient), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos, const glm::quat& orient, const glm::vec3& scale) : m_position(pos), m_orientation(orient), m_scale(scale) {}

	const glm::vec3& position() const { return m_position; }
	const glm::quat& orientation() const { return m_orientation; }
	const glm::vec3& scale() const { return m_scale; }

	glm::mat4 getModelMatrix()
	{
		glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_position);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
		glm::mat4 rotMatrix = glm::mat4_cast(m_orientation);
		return transMatrix * rotMatrix * scaleMatrix;
	}

	void translate(const glm::vec3 &v) { m_position += v; }
	void translate(float x, float y, float z) { m_position += glm::vec3(x, y, z); }

	void translateWithOrient(const glm::vec3 &v) {
		/*glm::quat orient = m_orientation;
		orient.x = 0;*/
		
		m_position += (v * glm::inverse(m_orientation));
	}

	void rotate(float angle, const glm::vec3 &axis) { m_orientation *= glm::angleAxis(angle, axis * m_orientation); }
	void rotate(float angle, float x, float y, float z) { m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation); }

	void scaleUp(const glm::vec3 &v) { m_scale += v; }
	void scaleUp(float x, float y, float z) { m_scale += glm::vec3(x, y, z); }
	void scaleDown(const glm::vec3 &v) { m_scale -= v; }

	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }
};
