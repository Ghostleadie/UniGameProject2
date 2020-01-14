#include "LevelEditor.h"
#include "Game.h"
#include "GameObject.h"
#include "InputHandler.h"

LevelEditor::LevelEditor()
{
	m_engineInterfacePtr = nullptr;

	m_sceneObject = new StaticGameObject;
	m_sceneObject->addComponent(new SceneComponent(&ModelMan));

	//initialise player
	ModelMan.addToVec("PlayerModel", "assets/models/PlayerModel2.obj");
	p1 = new player(ModelMan.fetchModel("PlayerModel"));

	//m_inputHandler.addGameObjectToListener(p1);
	m_inputHandler.addGameObjectToListener(m_sceneObject);
	
}

void LevelEditor::update(float dt)
{
	p1->OnUpdate(dt);
	for (int i = 0; i < m_sceneObject->getComponent<SceneComponent>()->v_dynamicLevelObjects.size(); i++) {
		m_sceneObject->getComponent<SceneComponent>()->v_dynamicLevelObjects[i].OnUpdate(dt);
	}
	for (int i = 0; i < v_gameObjects.size(); i++) {
		v_gameObjects[i]->OnUpdate(dt);
	}

}

void LevelEditor::render()
{
	m_engineInterfacePtr->renderColouredBackground(0, 0, 0);
	// update the camera (probably don't need to do this each frame)
	m_engineInterfacePtr->setCamera(p1->getComponent<CameraComponent>());

	//draw objects in current scene
	for (int i = 0; i < m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects.size(); i++)
	{
		m_engineInterfacePtr->drawModel(m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[i].getComponent<ModelComponent>()->theModel, m_sceneObject->getComponent<SceneComponent>()->v_staticLevelObjects[i].getComponent<TransformComponent>()->getModelMatrix());
	}
	for (int i = 0; i < m_sceneObject->getComponent<SceneComponent>()->v_dynamicLevelObjects.size(); i++) {
		m_engineInterfacePtr->drawModel(m_sceneObject->getComponent<SceneComponent>()->v_dynamicLevelObjects[i].getComponent<ModelComponent>()->theModel, m_sceneObject->getComponent<SceneComponent>()->v_dynamicLevelObjects[i].getComponent<TransformComponent>()->getModelMatrix());
	}
	m_engineInterfacePtr->drawModel(p1->getComponent<ModelComponent>()->theModel, p1->getComponent<TransformComponent>()->getModelMatrix());
	
}

bool LevelEditor::addModels(string name, string type,SceneComponent* scene) const {
	if (type == "StaticGameObject")
	{
		if (name == "floor") //id 0 in vector
		{
			StaticGameObject* newGameObject = new StaticGameObject();
			newGameObject->addComponent(new TransformComponent());// *trans = newGameObject2->getComponent<TransformComponent>();
			//ModelComponent* floor = newGameObject2->getComponent<ModelComponent>();
			Model* floor = new Model("assets/models/asphaltPlane.obj");
			newGameObject->addComponent(new ModelComponent(floor));
			scene->v_staticLevelObjects.push_back(*newGameObject);
			/*mm.load_model("floor.dae");*/
		}

		if (name == "hanger") {
			StaticGameObject* newGameObject1 = new StaticGameObject();
			newGameObject1->addComponent(new TransformComponent());// *trans = newGameObject2->getComponent<TransformComponent>();
			//ModelComponent* floor = newGameObject2->getComponent<ModelComponent>();
			Model* hanger = new Model("assets/models/hanger.obj");
			newGameObject1->addComponent(new ModelComponent(hanger));
			scene->v_staticLevelObjects.push_back(*newGameObject1);
		}

		if (name == "teapot") {
			StaticGameObject* newGameObject2 = new StaticGameObject();
			newGameObject2->addComponent(new TransformComponent());// *trans = newGameObject2->getComponent<TransformComponent>();
			//ModelComponent* floor = newGameObject2->getComponent<ModelComponent>();
			Model* teapot = new Model("assets/models/teapot.obj");
			newGameObject2->addComponent(new ModelComponent(teapot));
			scene->v_staticLevelObjects.push_back(*newGameObject2);
		}
		if (name == "grass") {
			StaticGameObject* newGameObject4 = new StaticGameObject();
			newGameObject4->addComponent(new TransformComponent());// *trans = newGameObject2->getComponent<TransformComponent>();
																   //ModelComponent* floor = newGameObject2->getComponent<ModelComponent>();
			Model* grass = new Model("assets/models/grassPlane.obj");
			newGameObject4->addComponent(new ModelComponent(grass));
			scene->v_staticLevelObjects.push_back(*newGameObject4);
		}
	}
	return true;
}