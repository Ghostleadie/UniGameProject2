#include "ModelManager.h"
#include <iostream>

Model* ModelManager::fetchModel(std::string modelName)
{
	for (int i = 0; i < modelVec.size(); i++)
	{
		if (modelVec[i].first == modelName)
		{
			return modelVec[i].second;
		}
	}
	return nullptr;
}

void ModelManager::addToVec(std::string modelName, std::string modelLoc)
{
	for (int i = 0; i < modelVec.size(); i++)
	{
		if (modelVec[i].first == modelName)
		{
			return;
		}
	}

	std::pair<std::string, Model*> newPair;
	newPair.first = modelName;
	newPair.second = new Model(modelLoc);
	modelVec.push_back(newPair);
}