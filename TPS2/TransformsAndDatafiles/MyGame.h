#pragma once
#include "Game.h"
#include "Player.h"
#include "AIPlayer.h"
#include "Physics.h"
#include "AIComponent.h"
#include "ParticleSystemRenderer.h"
#include "TransformComponent.h"
class MyGame : public Game
{
public:
	MyGame();
	//ParticleSystemRenderer particleRenderer;
	player* p1;
	std::vector<AIPlayer*> ai;
	ModelManager ModelMan;

	int numOfAIPlayers;
	void spawnAI(int aiCount);

	Physics phy;

	void update(float dt) override;
	void render() override;
	void SoundFilePaths();
	//void loadLevel(int iLevel);
	
	enum gameState {
		towerDefence,
		freeForAll,
		menu,
		endGame
	};

	gameState state;
	void print(int x, int y, int z, char *string);
	const char* soundFiles[1];

	int score;

private:
	
};