#pragma once
#include <vector>
#include "Model.h"

class ModelManager
{
public:
	ModelManager() {}; //CONSTRUCTOR
	Model* fetchModel(std::string modelName);
	void addToVec(std::string modelName, std::string modelLoc);
private:
	
	std::vector<std::pair<std::string, Model*>> modelVec;

};