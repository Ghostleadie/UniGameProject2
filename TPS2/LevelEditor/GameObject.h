#pragma once
#include "Component.h"
#include <unordered_map>
#include <typeindex>
#include "string"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "ModelComponent.h"

class GameObject
{
public:
	template <typename T>
	T* getComponent()
	{

		auto iter = m_components.find(typeid(T));

		if(iter != std::end(m_components))
		{
			// if found dynamic cast the component pointer and return it
			return dynamic_cast<T*>(iter->second);
		}
		
		// return null if we don't have a component of that type
		return nullptr;
		
	}

	template <typename T>
	void addComponent(T* comp)
	{
		// add the component to unoreder map with hash of its typeid
		m_components[typeid(T)] = comp;
	}
	
	virtual void OnUpdate(float dt) = 0;
	virtual void OnMessage(const std::string m) = 0;
	//virtual std::string getMessage() = 0;

	//std::vector<GameObject*>
	
	string name;
	const char* getName() {
		const char *charPointer = name.c_str();
		return charPointer;
	}

private:
	std::unordered_map<std::type_index, Component*> m_components;


};
