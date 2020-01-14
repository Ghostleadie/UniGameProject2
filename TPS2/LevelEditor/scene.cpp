#include "scene.h"


bool scene::loadScene(std::string levelFile)
{
	std::ifstream myInputFile;
	myInputFile.open(levelFile, std::ios_base::in);

	if (myInputFile.is_open())
	{
		ss.clear();

		std::getline(myInputFile, s);

		ss.str(s);
		ss.ignore(17);
		ss >> numOfCubes;

		float x, y, z, w;
		v_levelObjects.resize(numOfCubes);

		for (int i = 0; i < numOfCubes; i++)
		{
			ss.clear();
			std::getline(myInputFile, s);
			std::getline(myInputFile, s);
			ss.str(s);
			ss >> x >> y >> z;
			glm::vec3 pos(x, y, z);

			ss.clear();
			std::getline(myInputFile, s);
			ss.str(s);
			ss >> w >> x >> y >> z;
			glm::quat orientation(w, x, y, z);

			ss.clear();
			std::getline(myInputFile, s);
			ss.str(s);
			ss >> x >> y >> z;
			glm::vec3 scale(x, y, z);

			v_levelObjects[i].addComponent(new TransformComponent(pos, orientation, scale));
		}
		return true;
	}
	else {
		std::cout << "FAILED TO PARSE DATA";
		return false;
	}
}

bool scene::loadSceneJSON(std::string levelFile, ModelManager& modelMan)
{
	v_levelObjects.clear();

	std::fstream jsonData;
	Json::Value root;
	Json::Reader reader;

	jsonData.open(levelFile.c_str());

	if (!reader.parse(jsonData,root))
	{
		std::cout << "FAILED TO PARSE DATA";
		return false;
	}
	else {
		const Json::Value gameObjects = root["GameObjects"];
		v_levelObjects.resize(gameObjects.size());
		// size() is how big the array is
		for (int i = 0; i < gameObjects.size(); i++)
		{
			float x, y, z, w;

			const Json::Value modelName = gameObjects[i]["model"];
			Model* model = modelMan.fetchModel(modelName.asString());
			if (model == nullptr)
			{
				modelMan.addToVec(modelName.asString(), "assets/models/" + modelName.asString() + ".3ds");
				model = modelMan.fetchModel(modelName.asString());
			}


			//get position node
			const Json::Value posNode = gameObjects[i]["position"];

			x = posNode[0].asFloat();
			y = posNode[1].asFloat();
			z = posNode[2].asFloat();

			glm::vec3 pos(x, y, z);

			const Json::Value orientNode = gameObjects[i]["orientation"];

			w = orientNode[0].asFloat();
			x = orientNode[1].asFloat();
			y = orientNode[2].asFloat();
			z = orientNode[3].asFloat();

			glm::quat orient(w, x, y, z);

			const Json::Value scaleNode = gameObjects[i]["scale"];

			x = scaleNode[0].asFloat();
			y = scaleNode[0].asFloat();
			z = scaleNode[0].asFloat();

			glm::vec3 scale(x, y, z);

			v_levelObjects[i].addComponent(new TransformComponent(pos, orient, scale));
			v_levelObjects[i].addComponent(new ModelComponent(model));

			// get string
			std::cout << gameObjects[i]["name"].asString() << "loaded\n";

			//v_levelObjects[i].getComponent<TransformComponent>()->pitch(90);
			//v_levelObjects[i].getComponent<TransformComponent>()->roll(90);
			//v_levelObjects[i].getComponent<TransformComponent>()->yaw(90);
		}
		return true;
	}
}