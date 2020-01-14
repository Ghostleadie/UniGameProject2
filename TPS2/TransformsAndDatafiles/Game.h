#pragma once
#include "IEngineCore.h"
#include "InputHandler.h"
#include "GameObject.h"
#include "SceneComponent.h"



class Game
{
public:
	Game() {}
	virtual ~Game() {}
	IEngineCore* m_engineInterfacePtr;
	InputHandler m_inputHandler;
	GameObject* m_sceneObject;
	

	virtual void update(float dt) = 0;
	virtual void render() = 0;
};
