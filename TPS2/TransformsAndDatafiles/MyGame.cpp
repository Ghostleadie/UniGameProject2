#include "MyGame.h"
#include "AudioEngine.h"
#include "fmod_errors.h"
#include "irrKlang.h"
#include "ParticleComponent.h"

using namespace irrklang;

float soundCd = 0.5f;
float soundCdRm = 0;
float volume = 2;
float deltaTime = 1.0f / 60.0f;
double lastTime = glfwGetTime();
double currentTime;
ISoundEngine* engine = createIrrKlangDevice();

vec3df position(10, 5, 10);

MyGame::MyGame()
{
	m_engineInterfacePtr = nullptr;
	state = towerDefence;
	m_sceneObject = new StaticGameObject;
	m_sceneObject->addComponent(new SceneComponent(&ModelMan));

	//particleRenderer.initialise();

	//initialise player
	ModelMan.addToVec("PlayerModel", "assets/models/teapot.obj");
	p1 = new player(ModelMan.fetchModel("PlayerModel"));
	switch (state)
	{
	case towerDefence:
	{
		numOfAIPlayers = 8;
		break;
	}
	case freeForAll:
	{
		numOfAIPlayers = 3;
		break;
	}
	}

	spawnAI(numOfAIPlayers);

	m_inputHandler.addGameObjectToListener(p1);
	m_inputHandler.addGameObjectToListener(m_sceneObject);
}

void MyGame::update(float dt)
{

	//p1->OnUpdate(dt);
	soundCdRm -= deltaTime;
	
	if (p1->getComponent<TransformComponent>()->isMoving)
	{
		if (soundCdRm <= 0 && p1->getComponent<TransformComponent>()->m_position.x <= 24
			&& p1->getComponent<TransformComponent>()->m_position.x >= 0)
		{
			engine->play3D("assets/sounds/woodstomp.mp3", position, false);
			engine->setSoundVolume(0.75f);
			/*ISound* sound = engine->play3D("assets/sounds/woodstomp.mp3", position, false);
			if (sound)
			{
			sound->setMinDistance(10.0f);
			sound->setIsPaused(false);
			}*/
			soundCdRm = soundCd;
		}
		if (soundCdRm <= 0 && p1->getComponent<TransformComponent>()->m_position.x <= 0.1
			&& p1->getComponent<TransformComponent>()->m_position.x >= -24)
		{
			engine->play3D("assets/sounds/metalstomp.mp3", position, false);
			engine->setSoundVolume(0.75f);
			/*ISound* sound = engine->play3D("assets/sounds/woodstomp.mp3", position, false);
			if (sound)
			{
			sound->setMinDistance(10.0f);
			sound->setIsPaused(false);
			}*/
			soundCdRm = soundCd;
		}
	}

	switch (state)
	{
	case freeForAll :
	{


		//Update Player
		p1->OnUpdate(dt);
		//Update Player projectile collision
		for (int i = 0; i < p1->v_tea.size(); i++)
		{
			for (int j = 0; j < ai.size(); j++)
			{
				if (p1->v_tea[i]->lifeRemaining <= 0)
				{
					p1->v_tea.erase(p1->v_tea.begin());
					if (p1->v_tea.size() == 0 || ai.size() == 0)
					{
						j = ai.size();
					}
					else {
						i--;
					}

				}
				else if (phy.Teabagcoll(p1->v_tea[i]->getComponent<PhysicsComponent>(), ai[j]->getComponent<PhysicsComponent>()))
				{
					ai[j]->getComponent<AIComponent>()->OnMessage("rangedHit");
					p1->v_tea[i]->lifeRemaining = -1;
				}
			}
		}

		//update AI perception of other AIs and player
		bool s = false;
		for (int i = 0; i < ai.size(); i++)
		{
			s = false;
			ai[i]->getComponent<AIComponent>()->data->pData.v_go[0] = p1;
			ai[i]->getComponent<AIComponent>()->data->pData.currentTarget = p1;

			for (int j = 0; j < ai.size(); j++)
			{
				if (s)
				{
					ai[i]->getComponent<AIComponent>()->data->pData.v_go[j] = ai[j];
				}
				else if (ai[i] != ai[j] && s == false)
				{
					ai[i]->getComponent<AIComponent>()->data->pData.v_go[j + 1] = ai[j];
				}
				else {
					s = true;
				}
			}
		}

		//update AI and AI projectile collision
		for (int i = 0; i < ai.size(); i++)
		{
			ai[i]->OnUpdate(dt);

			for (int j = 0; j < ai[i]->v_tea.size(); j++)
			{
				if (ai[i]->v_tea[j]->lifeRemaining <= 0)
				{
					ai[i]->v_tea.erase(ai[i]->v_tea.begin());
					j--;
				}
				else if (ai[i]->v_tea[j] && phy.Teabagcoll(p1->getComponent<PhysicsComponent>(), ai[i]->v_tea[j]->getComponent<PhysicsComponent>()))
				{
					p1->OnMessage("rangedHit");
					ai[i]->v_tea[j]->lifeRemaining = -1;
				}
				else {
					for (int k = 0; k < ai.size(); k++)
					{
						if (ai[i]->v_tea[j] && ai[i] != ai[k] && phy.Teabagcoll(ai[i]->v_tea[j]->getComponent<PhysicsComponent>(), ai[k]->getComponent<PhysicsComponent>())) {
							ai[k]->getComponent<AIComponent>()->OnMessage("rangedHit");
							ai[i]->v_tea[j]->lifeRemaining = -1;
						}
					}
				}
			}
		}

		//updatge dynamic objects
		for (int i = 0; i < m_sceneObject->getComponent<SceneComponent>()->v_dynamicLevelObjects.size(); i++) {
			m_sceneObject->getComponent<SceneComponent>()->v_dynamicLevelObjects[i].OnUpdate(dt);
		}

		break;
	}

	case towerDefence :
	{
		SceneComponent* scene = m_sceneObject->getComponent<SceneComponent>();

		for (int i = 0; i < m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects.size(); i++) {
			if (phy.Teabagcoll(p1->getComponent<PhysicsComponent>(), scene->v_staticLevelObjects[i].getComponent<PhysicsComponent>())) {
				p1->getComponent<TransformComponent>()->m_position = p1->llllllll_lastpos;




			}

		}
		for (int i = 0; i < m_sceneObject->getComponent<SceneComponent>()->v_dynamicLevelObjects.size(); i++) {
			if (phy.AABB(p1->getComponent<PhysicsComponent>(), scene->v_dynamicLevelObjects[i].getComponent<PhysicsComponent>())) {
				
				p1->getComponent<TransformComponent>()->m_position = p1->ll_lastpos;


			}

		}
		//Update Player
		p1->OnUpdate(dt);
		//Update Player projectile collision
		for (int i = 0; i < p1->v_tea.size(); i++)
		{
			for (int j = 0; j < scene->v_staticLevelObjects.size(); j++)
			{
				if (p1->v_tea[i]->lifeRemaining <= 0)
				{
					p1->v_tea.erase(p1->v_tea.begin());
					j = scene->v_staticLevelObjects.size();
				}
				else if (phy.Teabagcoll(p1->v_tea[i]->getComponent<PhysicsComponent>(), scene->v_staticLevelObjects[j].getComponent<PhysicsComponent>()))
				{
					if (scene->v_staticLevelObjects[j].getComponent<AIComponent>())
					{
						scene->v_staticLevelObjects[j].getComponent<AIComponent>()->OnMessage("rangedHit");
						engine->play2D("assets/sounds/metalhit.wav", false);
					}
					p1->v_tea[i]->lifeRemaining = -1;
				}
			}
			if (p1->v_tea.size() > i )
			{
				for (int k = 0; k < scene->v_dynamicLevelObjects.size(); k++)
				{
					if (phy.AABB(p1->v_tea[i]->getComponent<PhysicsComponent>(), scene->v_dynamicLevelObjects[k].getComponent<PhysicsComponent>())) {
						//std::cout << "collision" << std::endl;
						p1->v_tea[i]->lifeRemaining = -1;
					}
				}
			}
			
		}

		//update AI perception of player
		//update AI and AI projectile collision

		int aiCheck  = 0;
		for (int i = 0; i < scene->v_staticLevelObjects.size(); i++)
		{
			if (scene->v_staticLevelObjects[i].getComponent<AIComponent>())
			{
				aiCheck++;
				scene->v_staticLevelObjects[i].getComponent<AIComponent>()->data->pData.v_go[0] = p1;
				
				//IF DISTANCE TO PLAYER < 10
				scene->v_staticLevelObjects[i].getComponent<AIComponent>()->data->pData.currentTarget = p1;
				//ELSE BLANK
				scene->v_staticLevelObjects[i].OnUpdate(dt);

				for (int j = 0; j < scene->v_staticLevelObjects[i].v_tea.size(); j++)
				{
					if (scene->v_staticLevelObjects[i].v_tea[j]->lifeRemaining <= 0)
					{
						scene->v_staticLevelObjects[i].v_tea.erase(scene->v_staticLevelObjects[i].v_tea.begin());
						if (scene->v_staticLevelObjects[i].v_tea.size() == 0)
						{
							j = 0;
						}
						else {
							j--;
						}
					}
					else if (scene->v_staticLevelObjects[i].v_tea[j] && phy.Teabagcoll(p1->getComponent<PhysicsComponent>(), scene->v_staticLevelObjects[i].v_tea[j]->getComponent<PhysicsComponent>()))
					{
						p1->OnMessage("rangedHit");
						scene->v_staticLevelObjects[i].v_tea[j]->lifeRemaining = -1;
					}
					else {
						for (int k = 0; k < scene->v_dynamicLevelObjects.size(); k++) {
							if (phy.AABB(scene->v_staticLevelObjects[i].v_tea[j]->getComponent<PhysicsComponent>(), scene->v_dynamicLevelObjects[k].getComponent<PhysicsComponent>())) {
								scene->v_staticLevelObjects[i].v_tea[j]->lifeRemaining = -1;
							}
						}
					}
				}
			}
		}

		if (aiCheck < numOfAIPlayers)
		{
			score += numOfAIPlayers - aiCheck;
			spawnAI(numOfAIPlayers - aiCheck);
			std::cout << score;
		}

		//update dynamic objects
		for (int i = 0; i < m_sceneObject->getComponent<SceneComponent>()->v_dynamicLevelObjects.size(); i++) {
			m_sceneObject->getComponent<SceneComponent>()->v_dynamicLevelObjects[i].OnUpdate(dt);
		}


		break;

	}
  }
}

void MyGame::render()
{
	if (p1->lives > 0)
	{
		m_engineInterfacePtr->renderColouredBackground(0, 0, 0);
		// update the camera (probably don't need to do this each frame)
		m_engineInterfacePtr->setCamera(p1->getComponent<CameraComponent>());
	//	m_engineInterfacePtr->setDefaultShaders();
		//draw objects in current scene
	
		//glUseProgram(m_engineInterfacePtr->m_defaultShaderProgram);
		for (int i = 0; i < m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects.size(); i++)
		{
			m_engineInterfacePtr->drawModel(m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[i].getComponent<ModelComponent>()->theModel, m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[i].getComponent<TransformComponent>()->getModelMatrix());
			if (m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[i].getComponent<AIComponent>())
			{
				for (int j = 0; j < m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[i].v_tea.size(); j++)
				{
					m_engineInterfacePtr->drawModel(m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[i].v_tea[j]->getComponent<ModelComponent>()->theModel, m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[i].v_tea[j]->getComponent<TransformComponent>()->getModelMatrix());
				}
			}
		}
	
		for (int i = 0; i < m_sceneObject->getComponent<SceneComponent>()->v_dynamicLevelObjects.size(); i++) {
			m_engineInterfacePtr->drawModel(m_sceneObject->getComponent<SceneComponent>()->v_dynamicLevelObjects[i].getComponent<ModelComponent>()->theModel, m_sceneObject->getComponent<SceneComponent>()->v_dynamicLevelObjects[i].getComponent<TransformComponent>()->getModelMatrix());
		}

		m_engineInterfacePtr->drawModel(p1->getComponent<ModelComponent>()->theModel, p1->getComponent<TransformComponent>()->getModelMatrix());
	
		for (int i = 0; i < p1->v_tea.size(); i++)
		{
			if (p1->v_tea[i])
			{
				m_engineInterfacePtr->drawModel(p1->v_tea[i]->getComponent<ModelComponent>()->theModel, p1->v_tea[i]->getComponent<TransformComponent>()->getModelMatrix());
			}
		}

		for (int i = 0; i < ai.size(); i++)
		{
			m_engineInterfacePtr->drawModel(ai[i]->getComponent<ModelComponent>()->theModel, ai[i]->getComponent<TransformComponent>()->getModelMatrix());
			for (int j = 0; j < ai[i]->v_tea.size(); j++)
			{
				if (ai[i]->v_tea[j])
				{
					m_engineInterfacePtr->drawModel(ai[i]->v_tea[j]->getComponent<ModelComponent>()->theModel, ai[i]->v_tea[j]->getComponent<TransformComponent>()->getModelMatrix());
				}
			}
		}


		//particleRenderer.render(getComponent<ParticleComponent>()->ParticlesCount,m_engineInterfacePtr->m_particleShaderProgram);
		
		
		//system("cls");
		std::cout << "\n\nYou Have " << p1->lives << " lives remaining";
		std::cout << "\n\nCurrent Score " << score;
	}
	else {
		m_engineInterfacePtr->renderColouredBackground(0, 0, 0);
		system("cls");
		std::cout << "\nYOU LOSE!" << std::endl;
		std::cout << "\nFinal Score : " << score;
	}
	

}


void MyGame::spawnAI(int aiCount) {
	switch (state)
	{
	case freeForAll:
	{
		for (int i = 0; i < aiCount; i++)
		{
			AIPlayer* p = new AIPlayer(ModelMan.fetchModel("PlayerModel"), glm::vec3(5 * i, 1, 5 * i));
			ai.push_back(p);
		}
		for (int i = 0; i < ai.size(); i++)
		{
			ai[i]->getComponent<AIComponent>()->data->pData.v_go.push_back(p1);
			for (int j = 0; j < ai.size(); j++)
			{
				if (ai[i] != ai[j])
				{
					ai[i]->getComponent<AIComponent>()->data->pData.v_go.push_back(ai[j]);
				}
			}
		}
		break;
	}
	case towerDefence:
	{
		
		for (int i = 0; i < aiCount; i++)
		{
			bool spawned = false;

			do
			{
				int r = rand() % m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects.size();
				if (!m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[r].getComponent<AIComponent>())
				{
					spawned = true;
					m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[r].addComponent(new AIComponent(m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[r].getComponent<TransformComponent>(), &m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[r]));
					m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[r].getComponent<AIComponent>()->data->pData.v_go.push_back(p1);
				}
			} while (!spawned);
		}

	}
	}
	
}

//void MyGame::print(int x, int y, int z, char *string)
//{
//
//	//set the position of the text in the window using the x and y coordinates
//	glRasterPos2f(x, y);
//	//get the length of the string to display
//	int len = (int)strlen(string);
//
//	//loop to display character by character
//	for (int i = 0; i < len; i++)
//	{
//		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str ing[i]);
//	}
//};