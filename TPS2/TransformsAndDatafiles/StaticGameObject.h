#pragma once
#include "GameObject.h"
#include "teabag.h"
class StaticGameObject : public GameObject
{
public:
	StaticGameObject();
	void OnUpdate(float dt) override;
	void OnMessage(const std::string m) override;
	vector<Teabag*> v_tea;
private:
	
	Model* tModel;
};