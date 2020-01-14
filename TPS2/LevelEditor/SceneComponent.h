#pragma once
#include "Component.h"
#include "ModelManager.h"
#include "JSON\json.h"
#include <sstream>
#include <fstream>
#include "GLFW_Imgui_EngineCore.h"
#include "StaticGameObject.h"
#include "DynamicGameObject.h"
#include "TransformComponent.h"
#include "ColourComponent.h"
#include "ModelComponent.h"


class SceneComponent : public Component
{
public:

	ModelManager* ModelMan;

	std::vector<std::string> levelVec;

	std::vector<StaticGameObject> v_staticLevelObjects;
	std::vector<DynamicGameObject> v_dynamicLevelObjects;



	SceneComponent(ModelManager* mMan)
	{
		ModelMan = mMan;

		if (!initialiseLevelList())
		{
			std::cout << "LEVEL LIST NOT INITIALISED\n";
		}

		loadSceneJSON(levelVec[3], *ModelMan);
	}

	void OnUpdate(float dt) override
	{

	}

	void OnMessage(const std::string m) override
	{
		if (m == "Set1")
		{
			if (!loadSceneJSON(levelVec[0], *ModelMan)) {
				std::cout << "FAILED TO LOAD " + levelVec[0] << std::endl;
			}
		}

		if (m == "Set2")
		{
			if (!loadSceneJSON(levelVec[1], *ModelMan)) {
				std::cout << "FAILED TO LOAD " + levelVec[1] << std::endl;
			}
		}

		if (m == "Set3")
		{
			if (!loadSceneJSON(levelVec[2], *ModelMan)) {
				std::cout << "FAILED TO LOAD " + levelVec[2] << std::endl;
			}
		}

		if (m == "Set4")
		{
			if (!loadSceneJSON(levelVec[3], *ModelMan)) {
				std::cout << "FAILED TO LOAD " + levelVec[3] << std::endl;
			}
		}

		if (m == "Set5")
		{
			v_staticLevelObjects.clear();
		}

	}

private:

	bool loadSceneJSON(std::string levelFile, ModelManager& modelMan) {
		v_staticLevelObjects.clear();
		v_dynamicLevelObjects.clear();
		std::fstream jsonData;
		Json::Value root;
		Json::Reader reader;

		jsonData.open("assets/levels/" + levelFile + ".json");

		if (!reader.parse(jsonData, root))
		{
			std::cout << "FAILED TO PARSE DATA\n";
			return false;
		}
		else {
			loadDynamicObjects(root, reader, modelMan);
			loadStaticObjects(root, reader, modelMan);

			std::cout << "LEVEL " + levelFile + " loaded.\n";
			return true;
		}
	}

	void loadDynamicObjects(Json::Value root, Json::Reader reader, ModelManager& modelMan) {
		const Json::Value dynamicGameObjects = root["DynamicGameObjects"];
		v_dynamicLevelObjects.resize(dynamicGameObjects.size());

		for (int i = 0; i < dynamicGameObjects.size(); i++)
		{
			float x, y, z, w;

			const Json::Value modelName = dynamicGameObjects[i]["model"];
			Model* model = modelMan.fetchModel(modelName.asString());
			if (model == nullptr)
			{
				modelMan.addToVec(modelName.asString(), "assets/models/" + modelName.asString());
				model = modelMan.fetchModel(modelName.asString());
			}


			//get position node
			const Json::Value posNode = dynamicGameObjects[i]["position"];

			x = posNode[0].asFloat();
			y = posNode[1].asFloat();
			z = posNode[2].asFloat();

			glm::vec3 pos(x, y, z);

			const Json::Value orientNode = dynamicGameObjects[i]["orientation"];

			w = orientNode[0].asFloat();
			x = orientNode[1].asFloat();
			y = orientNode[2].asFloat();
			z = orientNode[3].asFloat();

			glm::quat orient(w, x, y, z);

			const Json::Value scaleNode = dynamicGameObjects[i]["scale"];

			x = scaleNode[0].asFloat();
			y = scaleNode[1].asFloat();
			z = scaleNode[2].asFloat();

			glm::vec3 scale(x, y, z);

			const Json::Value typeNode = dynamicGameObjects[i]["type"];

			DynamicGameObject::type t;

			std::string type = typeNode.asString();
			if (type == "rotating")
			{
				t = DynamicGameObject::type::rotating;
			}
			if (type == "translating")
			{
				t = DynamicGameObject::type::translating;
			}
			if (type == "scaling")
			{
				t = DynamicGameObject::type::scaling;
			}

			v_dynamicLevelObjects[i].objectType = t;

			const Json::Value speedNode = dynamicGameObjects[i]["speed"];

			x = speedNode[0].asFloat();
			y = speedNode[1].asFloat();
			z = speedNode[2].asFloat();

			v_dynamicLevelObjects[i].speed = glm::vec3(x, y, z);

			const Json::Value upperLimitNode = dynamicGameObjects[i]["upperLimit"];

			x = upperLimitNode[0].asFloat();
			y = upperLimitNode[1].asFloat();
			z = upperLimitNode[2].asFloat();

			v_dynamicLevelObjects[i].limits.second = glm::vec3(x, y, z);

			const Json::Value lowerLimitNode = dynamicGameObjects[i]["lowerLimit"];

			x = lowerLimitNode[0].asFloat();
			y = lowerLimitNode[1].asFloat();
			z = lowerLimitNode[2].asFloat();

			v_dynamicLevelObjects[i].limits.first = glm::vec3(x, y, z);




			v_dynamicLevelObjects[i].addComponent(new ModelComponent(model));
			v_dynamicLevelObjects[i].addComponent(new TransformComponent(pos, orient, scale));


			// get string
			std::cout << dynamicGameObjects[i]["name"].asString() << "loaded\n";

			//v_staticLevelObjects[i].getComponent<TransformComponent>()->pitch(90);
			//v_staticLevelObjects[i].getComponent<TransformComponent>()->roll(90);
			//v_staticLevelObjects[i].getComponent<TransformComponent>()->yaw(90);
		}
	}

	void loadStaticObjects(Json::Value root, Json::Reader reader, ModelManager& modelMan) {
		const Json::Value staticGameObjects = root["StaticGameObjects"];
		v_staticLevelObjects.resize(staticGameObjects.size());
		// size() is how big the array is
		for (int i = 0; i < staticGameObjects.size(); i++)
		{
			float x, y, z, w;

			const Json::Value modelName = staticGameObjects[i]["model"];
			Model* model = modelMan.fetchModel(modelName.asString());
			if (model == nullptr)
			{
				modelMan.addToVec(modelName.asString(), "assets/models/" + modelName.asString());
				model = modelMan.fetchModel(modelName.asString());
			}


			//get position node
			const Json::Value posNode = staticGameObjects[i]["position"];

			x = posNode[0].asFloat();
			y = posNode[1].asFloat();
			z = posNode[2].asFloat();

			glm::vec3 pos(x, y, z);

			const Json::Value orientNode = staticGameObjects[i]["orientation"];

			w = orientNode[0].asFloat();
			x = orientNode[1].asFloat();
			y = orientNode[2].asFloat();
			z = orientNode[3].asFloat();

			glm::quat orient(w, x, y, z);

			const Json::Value scaleNode = staticGameObjects[i]["scale"];

			x = scaleNode[0].asFloat();
			y = scaleNode[0].asFloat();
			z = scaleNode[0].asFloat();

			glm::vec3 scale(x, y, z);

			v_staticLevelObjects[i].addComponent(new ModelComponent(model));
			v_staticLevelObjects[i].addComponent(new TransformComponent(pos, orient, scale));


			// get string
			std::cout << staticGameObjects[i]["name"].asString() << "loaded\n";

			//v_staticLevelObjects[i].getComponent<TransformComponent>()->pitch(90);
			//v_staticLevelObjects[i].getComponent<TransformComponent>()->roll(90);
			//v_staticLevelObjects[i].getComponent<TransformComponent>()->yaw(90);
		}
	}

	bool initialiseLevelList()
	{

		std::fstream jsonData;
		Json::Value root;
		Json::Reader reader;

		jsonData.open("assets/levels/levelList.json");

		if (!reader.parse(jsonData, root))
		{
			std::cout << "FAILED TO LOAD LEVEL DATA";
			return false;
		}
		else {
			const Json::Value levelList = root["Levels"];
			levelVec.resize(levelList.size());
			// size() is how big the array is
			for (int i = 0; i < levelList.size(); i++)
			{
				const Json::Value levelName = levelList[i]["levelName"];
				levelVec[i] = levelName.asString();
			}
			return true;
		}
	}

};
