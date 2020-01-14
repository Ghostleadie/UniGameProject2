#pragma once
#include "StaticGameObject.h"
#include <sstream>
#include <fstream>
#include "JSON\json.h"

#include "GLFW_EngineCore.h"

#include "TransformComponent.h"
#include "ColourComponent.h"
#include "ModelComponent.h"
#include "ModelManager.h"


class scene
{
public:
	bool loadScene(std::string levelFile);
	bool loadSceneJSON(std::string levelFile, ModelManager& modelMan);


	//void renderScene();
	
	std::vector<StaticGameObject> v_levelObjects;

private:	

	//for .txt load
	std::string s;
	std::stringstream ss;
	int numOfCubes;

};