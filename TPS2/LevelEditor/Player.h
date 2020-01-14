#pragma once
#include "DynamicGameObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"

class player : public GameObject
{
public:

	player()
	{

	}

	player(Model* modelIn);

	void OnUpdate(float dt) override;
	void OnMessage(const std::string m) override;




private:
	
	
	
};