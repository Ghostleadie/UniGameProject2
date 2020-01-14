#pragma once

#include "TransformComponent.h"
#include "Component.h"
#include <iostream>
#include "Behaviours.h"
#include "AIPlayer.h"

class AIComponent : public Component
{
public:
	attackRoot aRoot;
	bool alive = true;
	AIComponent() {}

	AIComponent(TransformComponent* transComp, GameObject* aiObj)
	{
		data = new AIData();
		data->healthState = healthy;
		data->type = balanced;
		data->currentHealth = 100;
		data->tComp = transComp;
		data->ai = aiObj;
		data->lives = 5;
	}
	//AIComponent(TransformComponent* transComp, AIPlayer& aiObj) {
	//	data = new AIData();
	//	data->healthState = healthy;
	//	data->type = balanced;
	//	data->currentHealth = 100;
	//	data->tComp = transComp;
	//	data->ai = new AIPlayer(aiObj);
	//}

	void OnMessage(const std::string m) override {
		if (m == "respawn")
		{
			data->healthState = healthy;
			data->currentHealth = 100;
			return;
		}

		if (m == "rangedHit")
		{
			data->currentHealth -= 25;
			std::cout << "RANGED HIT";
			return;
		}
		if (m == "meleeHit")
		{
			data->currentHealth -= 50;
			std::cout << "MELEE HIT";
			return;
		}
	}

	void OnUpdate(float dt) override {

		updateAITimers(dt);

		aRoot.run(data);


		//showDebug();
		switch (data->type)
		{
		case balanced:
		{

			break;
		}
		default:
			break;
		}
		//if (mt.run(tComp,dt)) {
		//	//std::cout << "ran\n";
		//}
		//else {
		//	std::cout << "AI failed\n";
		//}
		//mt.run(tComp, dt);
		/*glm::vec3 pos = tComp->m_position;
		if (pos.x < -10)
		{
			toggle = true;
		}
		if (pos.x > 10) {
			toggle = false;
		}
		if (toggle)
		{
			pos.x += 3 * dt;
			pos.z += 3 * dt;
		}
		else {
			pos.x -= 3 * dt;
			pos.z -= 3 * dt;
		}

		tComp->m_position = pos;*/
	}

	void updateAITimers(float dt) {
		//update health state 
		if (data->currentHealth <= 20)
		{
			data->healthState = fatal;
		}
		else if (data->currentHealth < 70)
		{
			data->healthState = hurt;
		}
		else {
			data->healthState = healthy;
		}

		//update ranged weapon data
		if (data->wData.reloading)
		{
			data->wData.reloadTimer += dt;
		}

		if (data->wData.delayTimer < data->wData.fireDelay)
		{
			data->wData.delayTimer += dt;
		}

		//update melee weapon data
		if (data->mData.delayTimer < data->mData.attackDelay)
		{
			data->mData.delayTimer += dt;
		}		
	}

	void updateAIData(TransformComponent* t) {
		data->tComp = t;
	}

	void showDebug() {
		system("cls");
		std::cout << "\nAI Pos: x -> " << data->tComp->m_position.x << ". z -> " << data->tComp->m_position.z;
		
		std::cout << "\n\nMelee Attack Data :\nMelee delay timer -> ";
		std::cout << data->mData.delayTimer;

		std::cout << "\n\nRanged Attack Data:\nRanged delay timer -> ";
		std::cout << data->wData.delayTimer;
		std::cout << "\nCurrent ammo count -> " << data->wData.ammoCount;

		std::cout << "\nReloading -> " << data->wData.reloading;
		if (data->wData.reloading) { std::cout << ". Time until reload -> " << data->wData.reloadTimer; }

		if (data->pData.currentTarget != nullptr)
		{
			std::cout << "\n\ntarget acquired.";// Distance to target -> " << checkDistance(data->pData.currentTarget->getComponent<TransformComponent>()->m_position, data->tComp->m_position);
		}

	}
	AIData* data;
private:
	
	

	//bool toggle = true;

};