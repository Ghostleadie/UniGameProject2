#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Component.h"

class CameraComponent : public Component
{
public:
	void OnUpdate(float dt) override
	{
		
	}

	void OnMessage(const std::string m) override
	{
		//if (m == "MoveForward")
		//{
		//	translate(glm::vec3(0, 0, 0.1));
		//}

		//if (m == "MoveBack")
		//{
		//	translate(glm::vec3(0, 0, -0.1));
		//}

		//if (m == "MoveLeft")
		//{
		//	translate(glm::vec3(0.1, 0, 0));
		//}

		//if (m == "MoveRight")
		//{
		//	translate(glm::vec3(-0.1, 0, 0));
		//}

		//if (m == "MoveUp")
		//{
		//	translate(glm::vec3(0, -0.1, 0));
		//}

		//if (m == "MoveDown")
		//{
		//	translate(glm::vec3(0, 0.1, 0));
		//}

		if (m == "resetCam")
		{
			m_position = glm::vec3(0, 0, 0);
			m_orientation = glm::quat(1, 0, 0, 0);
		}
	}

	glm::vec3 m_position;
	glm::quat m_orientation;
	float m_fov;
	bool firstPerson;

	CameraComponent() : m_position(0), m_orientation(1, 0, 0, 0), m_fov(45) {}
	CameraComponent(const glm::vec3& pos) : m_position(pos), m_orientation(1, 0, 0, 0), m_fov(45) {}
	CameraComponent(const glm::vec3& pos, const glm::quat& orient) : m_position(pos), m_orientation(orient), m_fov(45) {}

	void lookAt(const glm::vec3& target) { m_orientation = (glm::toQuat(glm::lookAt(m_position, target, glm::vec3(0, 1, 0)))); }

	const glm::vec3& position() const { return m_position; }
	const glm::quat& orientation() const { return m_orientation; }

	glm::mat4 getViewMatrix() const { return glm::translate(glm::mat4_cast(m_orientation), m_position); }

	void translate(const glm::vec3 &v) { m_position += v * m_orientation; }
	void translate(float x, float y, float z) { m_position += glm::vec3(x, y, z) * m_orientation; }

	void rotate(float angle, const glm::vec3 &axis) { m_orientation *= glm::angleAxis(angle, axis * m_orientation); }
	void rotate(float angle, float x, float y, float z) { m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation); }
	void rotate(glm::quat angle) { m_orientation *= angle; }

	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }

	void setFOV(float fov) { m_fov = fov; }
};

