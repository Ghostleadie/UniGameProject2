#pragma once
#include "Component.h"
#include "Model.h"

class ModelComponent : public Component
{
public:
	void OnUpdate(float dt) override {}

	void OnMessage(const std::string m) override {}

	ModelComponent();

	ModelComponent(Model* myModel) {
		theModel = myModel;
	}
	Model* theModel;
private:

};
