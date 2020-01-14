#pragma once
#include <map>
#include "GameObject.h"
#include <vector>
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "SceneComponent.h"

class InputCommand
{
public:
	virtual ~InputCommand() {}
	virtual void execute(GameObject& object) = 0;
	
};

//Camera Control (W,A,S,D,Q,E,R & Mouse)
class MoveCamForwardCommand : public InputCommand
{
public:
	void execute(GameObject& object) override {
		if (object.getComponent<CameraComponent>())
			object.getComponent<CameraComponent>()->OnMessage("MoveForward");
		if (object.getComponent <TransformComponent> ())
			object.getComponent<TransformComponent>()->OnMessage("MoveForward");
	}
};

class MoveCamBackCommand : public InputCommand
{
public:
	void execute(GameObject& object) override {
		if (object.getComponent<CameraComponent>())
			object.getComponent<CameraComponent>()->OnMessage("MoveBack");
		if (object.getComponent <TransformComponent>())
			object.getComponent<TransformComponent>()->OnMessage("MoveBack");
	}
};

class MoveCamRightCommand : public InputCommand
{
public:
	void execute(GameObject& object) override {
		if (object.getComponent<CameraComponent>())
			object.getComponent<CameraComponent>()->OnMessage("MoveRight");
		if (object.getComponent <TransformComponent>())
			object.getComponent<TransformComponent>()->OnMessage("MoveRight");
	}
};

class MoveCamLeftCommand : public InputCommand
{
public:
	void execute(GameObject& object) override {
		if (object.getComponent<CameraComponent>())
			object.getComponent<CameraComponent>()->OnMessage("MoveLeft");
		if (object.getComponent <TransformComponent>())
			object.getComponent<TransformComponent>()->OnMessage("MoveLeft");
	}
};

class MoveCamUpCommand : public InputCommand
{
public:
	void execute(GameObject& object) override {
		if (object.getComponent<CameraComponent>())
			object.getComponent<CameraComponent>()->OnMessage("MoveUp");
	}
};

class MoveCamDownCommand : public InputCommand
{
public:
	void execute(GameObject& object) override {
		if (object.getComponent<CameraComponent>())
			object.getComponent<CameraComponent>()->OnMessage("MoveDown");

	}
};

class RotateCam 
{
public:

	float midWindowX, midWindowY;
	int sensitivity = 50;

	RotateCam() {
		
	}

	void execute(GameObject& object, double mouseX, double mouseY, int windowX, int windowY) {
		midWindowX = windowX / 2;
		midWindowY = windowY / 2;

		float moveX = (mouseX - midWindowX) / sensitivity;
		float moveY = (mouseY - midWindowY) / sensitivity;
		
		if (object.getComponent<CameraComponent>()->firstPerson) {
			//object.getComponent<CameraComponent>()->yaw(moveX);
			//object.getComponent<CameraComponent>()->pitch(moveY);
			object.getComponent<TransformComponent>()->yaw(-moveX);
		}
		else {
			object.getComponent<TransformComponent>()->yaw(-moveX);
		}
		
		
		
		//glm::quat rotationQuat = glm::normalize(glm::quat(glm::vec3(moveY, moveX, 0)));

		//object.getComponent<CameraComponent>()->rotate(rotationQuat);
		
		/*object.getComponent<CameraComponent>()->yaw(moveX);
		object.getComponent<CameraComponent>()->pitch(moveY);*/

	}
};

class Reset : public InputCommand
{
public:
	void execute(GameObject& object) override {
		if (object.getComponent<CameraComponent>())
			object.OnMessage("reset");
	}
};

//Level Switch Controls (num 1 - 4)
class levelSet1 : public InputCommand
{
public:
	void execute(GameObject& object) override {
		if (object.getComponent<SceneComponent>())
			object.getComponent<SceneComponent>()->OnMessage("Set1");
	}
};

class levelSet2 : public InputCommand
{
public:
	void execute(GameObject& object) override {
		if (object.getComponent<SceneComponent>())
			object.getComponent<SceneComponent>()->OnMessage("Set2");
	}
};

class levelSet3 : public InputCommand
{
public:
	void execute(GameObject& object) override {
		if (object.getComponent<SceneComponent>())
			object.getComponent<SceneComponent>()->OnMessage("Set3");
	}
};

class levelSet4 : public InputCommand
{
public:
	void execute(GameObject& object) override {
		if (object.getComponent<SceneComponent>())
			object.getComponent<SceneComponent>()->OnMessage("Set4");
	}
};

//camera switch controls
class toggleCamera : public InputCommand
{
public:
	void execute(GameObject& object) override {
		if (object.getComponent<CameraComponent>())
			object.OnMessage("Switch");
	}
};



struct InputHandler
{	
	std::vector <GameObject*> v_gameObjects;
	std::map<int, InputCommand*> m_controlMapping;
	RotateCam camRotator;
	InputHandler()
	{
		// the idea will be to map the keys directly from a config file that can be loaded in
		// and changed on the fly
		//CAMERA CONTROLS
		m_controlMapping[65] = new MoveCamLeftCommand;		//A
		m_controlMapping[68] = new MoveCamRightCommand;		//D
		m_controlMapping[87] = new MoveCamForwardCommand;	//W
		m_controlMapping[83] = new MoveCamBackCommand;		//S
		m_controlMapping[81] = new MoveCamUpCommand;		//Q
		m_controlMapping[69] = new MoveCamDownCommand;		//E 
		m_controlMapping[82] = new Reset;					//R

		m_controlMapping[49] = new levelSet1;				//1
		m_controlMapping[50] = new levelSet2;				//2
		m_controlMapping[51] = new levelSet3;				//3
		m_controlMapping[52] = new levelSet4;				//4

		m_controlMapping[70] = new toggleCamera;			//F
	}

	void handleInputs(const std::vector<bool>& keyBuffer)
	{
		for(const auto& mapEntry : m_controlMapping)
		{
			if(keyBuffer[mapEntry.first])
			{
				for (int i = 0; i < v_gameObjects.size(); i++)
				{
					mapEntry.second->execute(*v_gameObjects[i]);
				}
			}
		}

	}

	void addGameObjectToListener(GameObject* objectPtr)
	{
		v_gameObjects.push_back(objectPtr);
	}

	 void handleMouseInput(double xpos, double ypos, int windowX, int windowY)
	{
		 for (int i = 0; i < v_gameObjects.size(); i++)
		 {
			 if(v_gameObjects[i]->getComponent<CameraComponent>())
				 camRotator.execute(*v_gameObjects[i], xpos, ypos, windowX, windowY);
		 }
		 
	}
};
