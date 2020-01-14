#include "StaticGameObject.h"
#include "AIComponent.h"
StaticGameObject::StaticGameObject() {
	tModel = new Model("assets/models/cube.obj");
}

void StaticGameObject::OnUpdate(float dt) {
	if (getComponent<AIComponent>())
	{
		AIComponent* a = getComponent<AIComponent>();
		if (a->alive)
		{
			//update Physics
			getComponent<PhysicsComponent>()->OnUpdate(dt, getComponent<TransformComponent>()->m_position);

			//getComponent<ParticleComponent>()->OnUpdate(dt,camPos,getComponent<TransformComponent>()->position());
			getComponent<AIComponent>()->updateAIData(getComponent<TransformComponent>());
			getComponent<AIComponent>()->OnUpdate(dt);
			if (getComponent<AIComponent>()->data->currentHealth <= 0) {
				destroyComponent<AIComponent>();
				return;
			}
			getComponent<AIComponent>()->data->ai = this;

		}
		else {
			/*respawnTimer += dt;
			if (respawnTimer >= respawnTime)
			{
				alive = true;
				glm::vec3 spawn(3, 1, 3);
				spawnAI(spawn);
			}*/
		}
		for (int i = 0; i < v_tea.size(); i++)
		{
			if (v_tea[i] != nullptr)
			{
				v_tea[i]->OnUpdate(dt);
			}
		}
	}
}

void StaticGameObject::OnMessage(const std::string m) {
	if (getComponent<AIComponent>())
	{
		if (m == "fire")
		{
			Teabag* t;
			t = new Teabag(tModel, getComponent<TransformComponent>()->m_position, getComponent<TransformComponent>()->m_orientation);
			//std::cout << "SHOTS FIRED!\n";
			v_tea.push_back(t);
		}
	}
}