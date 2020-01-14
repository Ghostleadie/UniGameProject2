//#include "Game.h"
//#include "TransformComponent.h"
//#include "ColourComponent.h"
//#include <fstream>
//#include <sstream>
//
//Game::Game()
//{
//	m_engineInterfacePtr = nullptr;
//	//m_inputHandler = new InputHandler(&m_playerCube);
//
//	m_player.addComponent(new RedComponent);
//	m_player.addComponent(new GreenComponent);
//	m_player.addComponent(new BlueComponent);
//
//	level = 1;
//	m_currentScene = new scene;
//
//	//m_playerCube.addComponent(new TransformComponent);
//	//m_secondCube.addComponent(new TransformComponent);
//	//// move the cube
//	//TransformComponent* temp = m_playerCube.getComponent<TransformComponent>();
//	//temp->translate(0, 0, -5);
//	//TransformComponent* temp2 = m_secondCube.getComponent<TransformComponent>();
//	//temp2->translate(1.5, 0, -5);
//
//	//loadLevel("assets/levels/myCubeLevel.txt");
//
//	m_currentScene->loadSceneJSON("assets/levels/myCubeLevel.json", ModelMan);
//
//}
//
//void Game::update()
//{
//	
//}
//
//void Game::render()
//{
//	float redValue = 0, greenValue = 0, blueValue = 0;
//	if(m_player.getComponent<RedComponent>())
//		redValue = m_player.getComponent<RedComponent>()->m_colourValue;
//	if (m_player.getComponent<GreenComponent>())
//		greenValue = m_player.getComponent<GreenComponent>()->m_colourValue;
//	if (m_player.getComponent<BlueComponent>())
//		blueValue = m_player.getComponent<BlueComponent>()->m_colourValue;
//	
//	// e.g. pass object details to the engine to render the next frame
//	m_engineInterfacePtr->renderColouredBackground(redValue, greenValue, blueValue);
//
//	// update the camera (probably don't need to do this each frame)
//	m_engineInterfacePtr->setCamera(&m_camera);
//
//	//draw cubes in current scene
//	
//	/*for (int i = 0; i < m_currentScene->v_levelObjects.size(); i++)
//	{
//		m_engineInterfacePtr->drawCube(m_currentScene->v_levelObjects[i].getComponent<TransformComponent>()->getModelMatrix());
//	}*/
//	if (level == 1)
//	{
//		for (int i = 0; i < m_currentScene->v_levelObjects.size(); i++)
//		{
//			m_engineInterfacePtr->drawModel(m_currentScene->v_levelObjects[i].getComponent<ModelComponent>()->theModel, m_currentScene->v_levelObjects[i].getComponent<TransformComponent>()->getModelMatrix());
//		}
//	}
//
//	// draw the cube
//	/*m_engineInterfacePtr->drawCube(m_playerCube.getComponent<TransformComponent>()->getModelMatrix());
//	
//	m_engineInterfacePtr->drawCube(m_secondCube.getComponent<TransformComponent>()->getModelMatrix());*/
//}
//
///*void Game::loadLevel(std::string levelFile)
//{
//	std::ifstream myInputFile;
//	myInputFile.open(levelFile, std::ios_base::in);
//
//	if (myInputFile.is_open())
//	{
//		ss.clear();
//		
//		std::getline(myInputFile, s);
//		
//		ss.str(s);
//		ss.ignore(17);
//		ss >> numOfCubes;
//	
//		float x, y, z, w;
//		v_levelObjects.resize(numOfCubes);
//
//		for (int i = 0; i < numOfCubes; i++)
//		{
//			ss.clear();
//			std::getline(myInputFile, s);
//			std::getline(myInputFile, s);
//			ss.str(s);
//			ss >> x >> y >> z;
//			glm::vec3 pos(x, y, z);
//
//			ss.clear();
//			std::getline(myInputFile, s);
//			ss.str(s);
//			ss >> w >> x >> y >> z;
//			glm::quat orientation(w, x, y, z);
//
//			ss.clear();
//			std::getline(myInputFile, s);
//			ss.str(s);
//			ss >> x >> y >> z;
//			glm::vec3 scale(x, y, z);
//
//			v_levelObjects[i].addComponent(new TransformComponent(pos, orientation, scale));
//		}
//
//	}
//
//
//
//
//}*/