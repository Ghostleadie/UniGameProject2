#include "physics.h"

Physics::Physics() {

}

/*glm::vec3 Physics::rayfromMouse(float mousex, float mousey, CameraComponent* cam) {
float x = (2.0f * mousex) / m_screenWidth - 1.0f;
float y = 1.0f - (2.0f * mousey) / m_screenHeight;
float z = 1.0f;
//normalised
glm::vec3 ray_norm = glm::vec3(x, y, z);
// Homogeneous Coordinates
glm::vec4 ray_Hom = glm::vec4(ray_norm.x, ray_norm.y, -1.0, 1.0);
//glm::inverse(ray_Hom);
glm::mat4 ray_proj = projection;
glm::inverse(ray_proj);
glm::vec4 ray_eye = ray_proj * ray_Hom;
ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0);
glm::mat4 ray_view = cam->getViewMatrix();
glm::inverse(ray_view);
glm::vec3 ray_world = ray_view * ray_eye;
ray_world = glm::normalize(ray_world);
return ray_world;
}*/





/*bool Physics::CircleCol(glm::vec3 obj1_centre_wor, glm::vec3 obj1_direction, glm::vec3 obj2_centre_wor, float planet_radius, float *intersection_distance) {
//first method

glm::vec3 Dist2coll = obj1_centre_wor - obj2_centre_wor;
//float a = glm::dot(ray_direction_wor, ray_direction_wor);
//float b = 2.0 * glm::dot(ray_direction_wor, (0.0f - Dist2Planet));
//float c = (glm::dot((0.0f - Dist2Planet), (0.0f - Dist2Planet))) - (planet_radius * planet_radius);
//float quad = (b * b) + (-4.0) *a *c;
//second

//float a = glm::dot(ray_direction_wor, ray_direction_wor);
float b = glm::dot(obj1_direction, (0.0f - Dist2coll));
float c = (glm::dot((0.0f - Dist2coll), (0.0f - Dist2coll))) - (planet_radius * planet_radius);
float quad = (b * b) - c;
//glm::vec3 glm::intersectRaySphere<>(ray_origin_wor, quad, planet_centre_wor, planet_radius, *intersection_distance);
//check if ray misses planet completely
if (quad < 0.0f) {
std::cout << "no collision" << std::endl;
return false;
}
//checking if ray hits twice
if (quad > 0.0f) {
glm::sqrt(quad);
//first
//float insect_a = (-b + quad) / (2.0 * a);
//float insect_b = (-b - quad) / (2.0 * a);
//second
float insect_a = -b + quad;
float insect_b = -b - quad;
*intersection_distance = insect_b;
//std::cout << "intersection b "<< insect_b << std::endl;;
//check if behind camera
if (insect_a < 0.0) {
if (insect_b < 0.0) {
std::cout << "collision detected" << std::endl;;
return false;
}
}
else if (insect_b < 0.0) {
*intersection_distance = insect_a;
//std::cout << "intersection b " << insect_a << std::endl;;
}
return true;
}
//skimming the edge
if (quad == 0.0f) {
float edge = -b + glm::sqrt(quad);
if (edge < 0.0f) {
return false;
}
*intersection_distance = edge;
std::cout << "collision detected" << edge << std::endl;;
return true;
}
return false;

}*/


bool Physics::Teabagcoll(PhysicsComponent* a, PhysicsComponent* b) {
	//float collision = ((b.m_position.x - a.m_position.x)*(b.m_position.x - a.m_position.x)) + ((obj2_centre_wor.z - obj1_centre_wor.z)*(obj2_centre_wor.z - obj1_centre_wor.z));
	glm::vec3 c = b->m_position - a->m_position;
	float collision = pow(c.x, 2) + pow(c.z, 2) + pow(c.y, 2);
	if (collision <= pow((a->radius + b->radius), 2)) {
		//std::cout << "collision" << std::endl;
		return true;
	}
	else return false;

}

bool Physics::AABB(PhysicsComponent* a, PhysicsComponent* b) {
	float aXmax = a->m_position.x + a->m_size.x;
	float aXmin = a->m_position.x - a->m_size.x;
	float bXmax = b->m_position.x + b->m_size.x;
	float bXmin = b->m_position.x - b->m_size.x;
	float aYmax = a->m_position.y + a->m_size.y;
	float aYmin = a->m_position.y - a->m_size.y;
	float bYmax = b->m_position.y + b->m_size.y;
	float bYmin = b->m_position.y - b->m_size.y;
	float aZmax = a->m_position.z + a->m_size.z;
	float aZmin = a->m_position.z - a->m_size.z;
	float bZmax = b->m_position.z + b->m_size.z;
	float bZmin = b->m_position.z - b->m_size.z;
	


	if ((aXmax > bXmin && aXmin < bXmax) && (aYmax > bYmin && aYmin < bYmax) && (aZmax > bZmin && aZmin < bZmax)) {
		std::cout << "collision" << std::endl;
		return true;
	}
	else {

		//std::cout << "missed" << std::endl;
		return false;
	}
}

bool Physics::CircleRec(PhysicsComponent* rec, PhysicsComponent* sph) {
	PhysicsComponent box = *rec;
	PhysicsComponent sphere = *sph;

	float x = max((box.m_position.x - box.m_size.x) , min(sphere.m_position.x, (box.m_position.x + box.m_size.x)));
	float y = max((box.m_position.y - box.m_size.y), min(sphere.m_position.y, (box.m_position.y + box.m_size.y)));
	float z = max((box.m_position.z - box.m_size.z), min(sphere.m_position.z, (box.m_position.z + box.m_size.z)));

	float dist = sqrt(pow((x - sphere.m_position.x), 2) + pow((y - sphere.m_position.y), 2) + pow((z - sphere.m_position.z), 2));
	if (dist < sphere.radius) {
		return true;
	}
	else return false;
}


bool Physics::pointcol(glm::vec3 point, PhysicsComponent sphere) {
	float dist = sqrt(pow((point.x - sphere.m_position.x), 2) + pow((point.y - sphere.m_position.y), 2) + pow((point.z - sphere.m_position.z), 2));

	if (dist < sphere.radius) {
		return true;
	}
	else return false;
}


/*void Physics::glfw_mouse_click_callback(GLFWwindow *window, Game& game, Player& player1) {
//ImGuiIO& io = ImGui::GetIO();
//if (io.MouseClicked[0]) {
std::cout << "mouse clicked" << std::endl;;
double mousexpos, mouseypos;
glfwGetCursorPos(window, &mousexpos, &mouseypos);

glm::vec3 ray_war = rayfromMouse((float)mousexpos, (float)mouseypos, player1.getComponent<CameraComponent>());

int closestPlanet = -1;
float closestInsec = 0.0f;
int tsize = game.m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects.size();
for (int i = 0; i < tsize; i++) {
float t_dist = 0.0f;
if (CircleCol(player1.getComponent<CameraComponent>()->m_position, ray_war, game.m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[i].getComponent<TransformComponent>()->position(), objectradius, &t_dist)) {
if (closestPlanet == -1 || t_dist < closestInsec) {
closestPlanet = i;
//std::cout << "closest planet: " << closestPlanet << std::endl;
closestInsec = t_dist;
//std::cout << "closest intersection: " << closestInsec << std::endl;
}
}
}
objectclick = closestPlanet;
if (objectclick != -1 && objectclick != 0) {
//std::string name = game.m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[objectclick].getName();
//std::cout << name << std::endl;
//glm::vec3 a = game.m_currentScene->player.getComponent<CameraComponent>()->m_position;

//glm::intersectRaySphere()
}
//}
}*/


/*bool Physics::CircleCol(glm::vec3 obj1_centre_wor, glm::vec3 obj1_direction, glm::vec3 obj2_centre_wor, float planet_radius, float *intersection_distance) {
	//first method

	glm::vec3 Dist2collision = obj1_centre_wor - obj2_centre_wor;
	//float a = glm::dot(ray_direction_wor, ray_direction_wor);
	//float b = 2.0 * glm::dot(ray_direction_wor, (0.0f - Dist2Planet));
	//float c = (glm::dot((0.0f - Dist2Planet), (0.0f - Dist2Planet))) - (planet_radius * planet_radius);
	//float quad = (b * b) + (-4.0) *a *c;
	//second

	//float a = glm::dot(ray_direction_wor, ray_direction_wor);
	float b = glm::dot(obj1_direction, (0.0f - Dist2collision));
	float c = (glm::dot((0.0f - Dist2collision), (0.0f - Dist2collision))) - (planet_radius * planet_radius);
	float quad = (b * b) - c;
	//glm::vec3 glm::intersectRaySphere<>(ray_origin_wor, quad, planet_centre_wor, planet_radius, *intersection_distance);
	//check if ray misses planet completely
	if (quad < 0.0f) {
		std::cout << "missed" << std::endl;
		return false;
	}
	//checking if ray hits twice
	if (quad > 0.0f) {
		glm::sqrt(quad);
		//first
		//float insect_a = (-b + quad) / (2.0 * a);
		//float insect_b = (-b - quad) / (2.0 * a);
		//second
		float insect_a = -b + quad;
		float insect_b = -b - quad;
		*intersection_distance = insect_b;
		//std::cout << "intersection b "<< insect_b << std::endl;;
		//check if behind camera
		if (insect_a < 0.0) {
			if (insect_b < 0.0) {
				std::cout << "missed" << std::endl;;
				return false;
			}
		}
		else if (insect_b < 0.0) {
			*intersection_distance = insect_a;
			//std::cout << "intersection b " << insect_a << std::endl;;
		}
		return true;
	}
	//skimming the edge
	if (quad == 0.0f) {
		float edge = -b + glm::sqrt(quad);
		if (edge < 0.0f) {
			return false;
		}
		*intersection_distance = edge;
		std::cout << "edge " << edge << std::endl;;
		return true;
	}
	return false;
}

//bool Physics::jump(TransformComponent* pos) {

//	return true;
//}

float Physics::distance(glm::vec3 a, glm::vec3 b) {
	float distance = sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
	return distance;
}*/