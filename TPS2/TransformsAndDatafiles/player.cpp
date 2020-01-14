#include "Player.h"
#include <iostream>
#include "irrKlang.h"

using namespace irrklang;

ISoundEngine* engine2 = createIrrKlangDevice();

player::player(Model* modelIn) {
	
	addComponent(new ModelComponent(modelIn));
	addComponent(new TransformComponent(glm::vec3(0, 1, 0),glm::quat(1, 0, 0, 0)));
	addComponent(new CameraComponent(glm::vec3(0, 0, 0), glm::quat(1, 0, 0, 0)));
	addComponent(new PhysicsComponent(glm::vec3(0, 1, 0), 1, glm::vec3(0, 0, 0)));
	getComponent<CameraComponent>()->firstPerson = true;
	tModel = new Model("assets/models/sugar.obj");
}

void player::OnUpdate(float dt) {

	getComponent<TransformComponent>()->OnUpdate(dt);

		if (health <= 0 && alive)
		{
			alive = false;
			lives -= 1;
		}

	if (alive)
	{
		llllllll_lastpos = lllllll_lastpos;
		lllllll_lastpos = llllll_lastpos;
		llllll_lastpos = lllll_lastpos;
		lllll_lastpos = llll_lastpos;
		llll_lastpos = lll_lastpos;
		lll_lastpos = ll_lastpos;
		ll_lastpos = l_lastpos;
		l_lastpos = lastpos;
		lastpos = getComponent<TransformComponent>()->m_position;
		getComponent<PhysicsComponent>()->OnUpdate(dt, getComponent<TransformComponent>()->m_position);

			if (delayTimer < fireDelay)
			{

				delayTimer += dt;
			}
			else {
				canShoot = true;
			}

			for (int i = 0; i < v_tea.size(); i++)
			{
				if (v_tea[i] != nullptr)
				{
					v_tea[i]->OnUpdate(dt);
				}
			}
		}
		else {
			respawnTimer += dt;
			getComponent<TransformComponent>()->m_orientation = glm::quat(0.707, -0.707, 0, 0);
			getComponent<TransformComponent>()->m_position = glm::vec3(0, 60, 0);

			if (respawnTimer > respawnTime)
			{

				getComponent<TransformComponent>()->m_orientation = glm::quat(1, 0, 0, 0);
				getComponent<TransformComponent>()->m_position = glm::vec3(0, 1, 0);
				health = 100;
				alive = true;
				respawnTimer = 0;
			}
		}

		if (getComponent<CameraComponent>()->firstPerson)
		{
			TransformComponent* transform = getComponent<TransformComponent>();

			getComponent<CameraComponent>()->m_position = -(transform->m_position + (glm::vec3(0, 0, -1.5) * glm::inverse(transform->m_orientation)));

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
	/*if (alive)
	{
		
	if (m == "MoveForward")
	{
		getComponent<TransformComponent>()->translateWithOrient(glm::vec3(0, 0, -0.1));
	}

	if (m == "MoveBack")
	{
		getComponent<TransformComponent>()->translateWithOrient(glm::vec3(0, 0, 0.1));
	}

	if (m == "MoveLeft")
	{
		getComponent<TransformComponent>()->translateWithOrient(glm::vec3(-0.1, 0, 0));
	}

	if (m == "MoveRight")
	{
		getComponent<TransformComponent>()->translateWithOrient(glm::vec3(0.1, 0, 0));
	}

	if (m == "MoveUp")
	{
		getComponent<TransformComponent>()->translate(glm::vec3(0, -0.1, 0));
	}

	if (m == "MoveDown")
	{
		getComponent<TransformComponent>()->translate(glm::vec3(0, 0.1, 0));
	}
		

	if (m == "resetCam")
	{
		getComponent<TransformComponent>()->m_position = glm::vec3(0, 0, 0);
		getComponent<TransformComponent>()->m_orientation = glm::quat(1, 0, 0, 0);
	}*/
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


	if (m == "rangedHit")
	{
		health -= 25;
		engine2->play2D("assets/sounds/metalhit.wav", false);
		std::cout << "\n PLAYER HIT FROM RANGE \n";
	}

	if (m == "meleeHit")
	{
		health -= 50;
		engine2->play2D("assets/sounds/metalhit.wav", false);
	}

	if (m == "fire")
	{
		if (canShoot)
		{

			Teabag* t;

			TransformComponent* tComp = getComponent<TransformComponent>();

			t = new Teabag(tModel, tComp->m_position, tComp->m_orientation);

			v_tea.push_back(t);

			engine2->play2D("assets/sounds/attack.wav", false);
			//engine->setSoundVolume(0.1f);

			canShoot = false;
			delayTimer = 0;

		}
	}
	//}
}