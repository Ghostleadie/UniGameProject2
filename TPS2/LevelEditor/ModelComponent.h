#pragma once
#include "Component.h"
#include "Model.h"

class ModelComponent : public Component
{
public:
	void OnUpdate(float dt) override {}

	void OnMessage(const std::string m) override {}

	ModelComponent();

	Model* theModel;

	ModelComponent(Model* myModel) {
		theModel = myModel;
	}
	Model* getModel() { return theModel; }
private:

};
