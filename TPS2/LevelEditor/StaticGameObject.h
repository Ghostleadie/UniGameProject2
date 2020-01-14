#pragma once
#include "GameObject.h"

class StaticGameObject : public GameObject
{
public:
	StaticGameObject();
	void OnUpdate(float dt) override;
	void OnMessage(const std::string m) override;

	string name;
	const char* getName() {
		const char *charPointer = name.c_str();
		return charPointer;
	}
private:

};