#include "AIPlayer.h"
#include "AIComponent.h"
AIPlayer::AIPlayer(Model* modelIn, glm::vec3 pos)
{
	addComponent(new ModelComponent(modelIn));
	addComponent(new TransformComponent(pos, orient));
	addComponent(new PhysicsComponent(pos,1,glm::vec3(0,0,0)));
	TransformComponent* tComp = getComponent<TransformComponent>();

	AIPlayer* a = this;
	tModel = new Model("assets/models/sugar.obj");

	addComponent(new AIComponent(tComp,a));
	spawnAI(pos);
	//addComponent(new ParticleComponent());

	//getComponent<ParticleComponent>()->start();
}

void AIPlayer::OnUpdate(float dt)
{
	if (alive)
	{
		//update Physics
		getComponent<PhysicsComponent>()->OnUpdate(dt, getComponent<TransformComponent>()->m_position);

		//getComponent<ParticleComponent>()->OnUpdate(dt,camPos,getComponent<TransformComponent>()->position());
		getComponent<AIComponent>()->updateAIData(getComponent<TransformComponent>());
		getComponent<AIComponent>()->OnUpdate(dt);
		if (getComponent<AIComponent>()->data->currentHealth <= 0) {
			destroyAI();
		}
		getComponent<AIComponent>()->data->ai = this;
		
	}
	else {
		respawnTimer += dt;
		if (respawnTimer >= respawnTime)
		{
			alive = true;
			glm::vec3 spawn(3, 1, 3);
			spawnAI(spawn);
		}
	}
	for (int i = 0; i < v_tea.size(); i++)
	{
		if (v_tea[i] != nullptr)
		{
			v_tea[i]->OnUpdate(dt);
		}
	}
}

void AIPlayer::OnMessage(const std::string m)
{

	if (m == "drawParticles")
	{
		//getComponent<ParticleComponent>()->draw();
	}

	if (m == "fire")
	{
		Teabag* t;
		
		t = new Teabag(tModel, getComponent<TransformComponent>()->m_position, getComponent<TransformComponent>()->m_orientation);
		//std::cout << "SHOTS FIRED!\n";
		v_tea.push_back(t);
	}
}

void AIPlayer::destroyAI()
{
	alive = false;
	respawnTimer = 0;
	TransformComponent* tComp = getComponent<TransformComponent>();
	tComp->m_position = glm::vec3(0,-30,0);
}

void AIPlayer::spawnAI(glm::vec3 pos) {

	orient.w = 1;
	orient.x = 0;
	orient.y = 0;
	orient.z = 0;
	
	TransformComponent* tComp = getComponent<TransformComponent>();
	tComp->m_position = pos;
	tComp->m_orientation = orient;

	respawnTimer = 0;
	getComponent<AIComponent>()->OnMessage("respawn");
}
