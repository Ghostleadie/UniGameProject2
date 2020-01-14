#pragma once
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CameraComponent.h"
#include "GLFW_EngineCore.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"

class Physics {
public:
	Physics();
	//glm::vec3 rayfromMouse(float mousex, float mousey, CameraComponent* cam);
	//bool CircleCol(glm::vec3 obj1_centre_wor, glm::vec3 obj1_direction, glm::vec3 obj2_centre_wor, float planet_radius, float *intersection_distance);
	bool Teabagcoll(PhysicsComponent* a, PhysicsComponent* b);
	bool AABB(PhysicsComponent* box, PhysicsComponent* sphere);
	bool CircleRec(PhysicsComponent* a, PhysicsComponent* b);
	bool pointcol(glm::vec3 point, PhysicsComponent sphere);
	//bool jump(TransformComponent pos);
	float distance(glm::vec3 a, glm::vec3 b);
	//void glfw_mouse_click_callback(GLFWwindow *window, Game& game, player& player);
	glm::vec3 lastpos;
	float objectradius = 2.0f;
	int objectclick;
};