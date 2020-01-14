#pragma once
#include "GameObject.h"

class MiscGameObject : public GameObject
{
public:
	MiscGameObject();
	void OnUpdate(float dt) override;
	void OnMessage(const std::string m) override;

private:

};