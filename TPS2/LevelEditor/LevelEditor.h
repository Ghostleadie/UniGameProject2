#pragma once
#include "Game.h"
#include "Player.h"
#include "GameObject.h"
#include "SceneComponent.h"

class LevelEditor : public Game
{

public:
	LevelEditor();
	
	player* p1;
	ModelManager ModelMan;
	
	bool addModels(string name, string type, SceneComponent* scene) const;
	void update(float dt) override;
	void render() override;
	//void loadLevel(int iLevel);
	
	std::vector<GameObject*>v_gameObjects;


private:
	
};