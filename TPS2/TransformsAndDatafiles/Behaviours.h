#pragma once

#include "Nodes.h"
#include "Pathfinding.h"
#include "AIStructs.h"
#include "Teabag.h"
#include "model.h"
#include "Physics.h"
//
//class AIBehaviour : public CompositeNode {
//	Sequence* root = new Sequence;
//	Selector* moveSelector = new Selector;
//};
//
//class test : public AINode {
//	bool run() override {
//		return false;
//	}
//};
//
//class test2 : public AINode {
//	bool run() override {
//		return false;
//	}
//	
//};
//
//class test3 : public AINode {
//	bool run() override {
//		return true;
//	}
//};
//
//class testSequence : public Sequence {
//public:	
//	testSequence() {
//		AINode* n = new test;
//		AINode* m = new test2;
//		addChild(n);
//		addChild(m);
//	}
//};
//
//class testSequence2 : public Sequence {
//public:
//	testSequence2() {
//		AINode* n = new test2;
//		AINode* m = new test3;
//		addChild(n);
//		addChild(m);
//	}
//};
//
//class testSelector : public Selector {
//public:
//	testSelector() {
//		AINode* a = new testSequence;
//		AINode* b = new testSequence2;
//		addChild(a);
//		addChild(b);
//	}
//};
//
//class testBehaviour : public CompositeNode {
//public:
//	Sequence* root = new Sequence;
//	Selector* selector = new testSelector;
//
//	testBehaviour() {
//		root->addChild(selector);
//	}
//
//	bool run() override {
//		if (root->run()) {
//			return true;
//		}
//		return false;
//	}
//
//};


//class attackDecisionTree;
//
//class CheckHealthTask : public CompositeNode
//{
//	bool run(AIData* a) override{}
//
//	bool run(float health, healthState s)
//	{
//		if (health <= 20)
//		{
//			s = fatal;
//		}
//		else if (health < 70)
//		{
//			s = hurt;
//		}
//		else {
//			s = healthy;
//		}
//		return true;
//	}
//};
//
//class simpleMoveTask : public AINode
//{
//public:
//	bool toggle = false;
//	bool run(AIData* a) override
//	{
//		return false;
//	}
//
//	bool run(TransformComponent* tComp, float dt)
//	{
//		glm::vec3 pos = tComp->m_position;
//		if (pos.x < -10)
//		{
//			toggle = true;
//		}
//		if (pos.x > 10) {
//			toggle = false;
//		}
//		if (toggle)
//		{
//			pos.x += 3 * dt;
//			pos.z += 3 * dt;
//		}
//		else {
//			pos.x -= 3 * dt;
//			pos.z -= 3 * dt;
//		}
//
//		tComp->m_position = pos;
//
//		return true;
//	}
//
//
//};

class lineOfSightTask : public CompositeNode {
	bool run(AIData* a) override {

	}
};






class checkLineOfSight : public AINode {
	bool run(AIData* a) override {
		//CHECK LOS
		if (sqrt(pow((a->tComp->m_position.x - a->pData.currentTarget->getComponent<TransformComponent>()->m_position.x), 2) + pow((a->tComp->m_position.y - a->pData.currentTarget->getComponent<TransformComponent>()->m_position.y), 2) + pow((a->tComp->m_position.z - a->pData.currentTarget->getComponent<TransformComponent>()->m_position.z), 2)) < 20) {
			return true;
		}
		return false;
	}
};

//Melee Attack Branch
class checkMeleeRange : public AINode {

	bool run(AIData* a) override {
		//check range < 0.5
		if (sqrt(pow((a->tComp->m_position.x - a->pData.currentTarget->getComponent<TransformComponent>()->m_position.x), 2) + pow((a->tComp->m_position.y - a->pData.currentTarget->getComponent<TransformComponent>()->m_position.y), 2) + pow((a->tComp->m_position.z - a->pData.currentTarget->getComponent<TransformComponent>()->m_position.z), 2)) < 0.5) {
			return true;
		}
		return false;
	}
};

class checkMeleeDelay : public AINode {

	bool run(AIData* a) override {
		if (a->mData.delayTimer < a->mData.attackDelay)
		{
			return false;
		}
		return true;
	}
};

class makeMeleeAttack : public AINode {

	bool run(AIData* a) override {
		a->pData.currentTarget->OnMessage("meleeHit");
		a->mData.delayTimer = 0;
		return true;
	}
};

class meleeAttack : public Sequence
{
public:
	meleeAttack() {
		AINode* hasLOS = new checkLineOfSight;
		AINode* range = new checkMeleeRange;
		AINode* meleeDelay = new checkMeleeDelay;
		AINode* attack = new makeMeleeAttack;

		addChild(hasLOS);
		addChild(range);
		addChild(meleeDelay);
		addChild(attack);
	}
};

//Ranged Attack Branch

class hasAmmoTask : public AINode {
	bool run(AIData* a) override {

		if (a->wData.ammoCount == 0)
		{
			return false;
		}
		return true;
	}
};

class checkFireDelayTask : public AINode {
	bool run(AIData* a) override {
		if (a->wData.delayTimer < a->wData.fireDelay)
		{
			return false;
		}
		return true;
	}
};

class aimTask : public AINode {
	bool run(AIData* a) override
	{
		glm::quat* q1 = new glm::quat(a->tComp->orientation());
		glm::vec3 i = a->tComp->m_position;
		glm::vec3 j = a->pData.currentTarget->getComponent<TransformComponent>()->m_position; 
		//glm::vec2 i2(i.x, i.z);
		//glm::vec2 j2(j.x, j.z);

		//float dot = glm::dot(i, j);
		////float dot = ((i.x * j.x) + (i.y * j.y) + (i.z * j.z));
		//float mi = sqrt(pow(i.x, 2) + pow(i.y, 2) + pow(i.z, 2));
		//float mj = sqrt(pow(j.x, 2) + pow(j.y, 2) + pow(j.z, 2));
		//float th = acos(dot / (mi * mj));
		//
		//glm::vec3 axis = cross(i,j);
		//a->ai->getComponent<TransformComponent>()->rotate(th, axis);
		
		//float dot = glm::dot(i2,j2);
		
		float th = atan2((i.z - j.z), (i.x - j.x)) * (180 / 3.141);
		
		th = (th - (floor(th / 360) * 360));

		
		//std::cout << "\nangle " << th;

		th -= 90;
		th *= 3.141592f / 180.0f;

		
		glm::quat y = glm::angleAxis(-th, glm::vec3(0, 1, 0));
		glm::quat x = glm::angleAxis(0.f, glm::vec3(1, 0, 0));
		glm::quat z = glm::angleAxis(0.f, glm::vec3(0, 0, 1));



		//float x, y, z;

		//if (q1->w > 1) glm::normalize(q1); // if w>1 acos and sqrt will produce errors, this cant happen if quaternion is normalised
		//float angle = 2 * acos(q1->w);
		//double s = sqrt(1 - q1->w*q1->w); // assuming quaternion normalised then w is less than 1, so term always positive.
		//if (s < 0.001) { // test to avoid divide by zero, s is always positive due to sqrt
		//				 // if s close to zero then direction of axis not important
		//	x = q1->x; // if it is important that axis is normalised then replace with x=1; y=z=0;
		//	y = q1->y;
		//	z = q1->z;
		//}
		//else {
		//	x = q1->x / s; // normalise axis
		//	y = q1->y / s;
		//	z = q1->z / s;
		//}


		a->ai->getComponent<TransformComponent>()->m_orientation = y * x * z;

		return true;
	}
};

class fireTask : public AINode {

	bool run(AIData* a) override
	{
		
		a->ai->OnMessage("fire");
		a->wData.delayTimer = 0;
		a->wData.ammoCount -= 1;
		return true;
	}

};

class rangedAttack : public Sequence
{
public:
	rangedAttack() {
		AINode* hasLOS = new checkLineOfSight;
		AINode* ammo = new hasAmmoTask;
		AINode* aim = new aimTask;
		AINode* fireDelay = new checkFireDelayTask;
		AINode* fire = new fireTask;

		addChild(hasLOS);
		addChild(ammo);
		addChild(aim);
		addChild(fireDelay);
		addChild(fire);

	}
};

//Reload Branch
class checkAmmo : public AINode {
	bool run(AIData* a) override {
		if (a->wData.ammoCount < 3 && a->state != roaming)
		{
			return true;
		}
		else if(a->state == roaming) {
			return true;
		}
		return false;
		
	}
};

class reloadTask : public AINode {

	bool run(AIData* a) override
	{
		if (a->wData.ammoCount == 0 && !a->wData.reloading)
		{
			a->wData.reloading = true;
		}
		else if (a->wData.reloading)
		{
			if (a->wData.reloadTimer > a->wData.reloadTime)
			{
				a->wData.reloading = false;
				a->wData.ammoCount = a->wData.magSize;
				a->wData.delayTimer = 0;
				a->wData.reloadTimer = 0;
			}
		}
		return true;
	}

};


class reloadSequence : public Sequence
{
public:
	reloadSequence() {
		AINode* ammoCheck = new checkAmmo;
		AINode* reload = new reloadTask;

		addChild(ammoCheck);
		addChild(reload);
	}
};

//ATTACK AI ROOT


class AttackSequence : public Selector {
public:
	AttackSequence() {
		//AINode* clos = new checkLineOfSight;
		AINode* melee = new meleeAttack;
		AINode* range = new rangedAttack;

		//addChild(clos);
		addChild(melee);
		addChild(range);
	}
};

class defaultBehaviour : public AINode {
	bool run(AIData* a) override {
		return true;
	}
};

class attackDecisionTree : public Selector {
public:
	attackDecisionTree() {
		AINode* attack = new AttackSequence;
		AINode* reload = new reloadSequence;
		AINode* dBehaviour = new defaultBehaviour;

		addChild(attack);
		addChild(reload);
		addChild(dBehaviour);
	}
};

class attackRoot : public Sequence {
public:
	attackRoot() {
		AINode* attackSelector = new attackDecisionTree;
		addChild(attackSelector);
	}
};
//MOVE AI TREE




class evasionSequence : public Sequence {
public:
	evasionSequence() {

	}

};

class moveDecisionTree : public Selector {
public:
	moveDecisionTree() {

	}
};

//MOVE AI ROOT
class moveRoot : public Sequence {
public:
	moveRoot() {
		AINode* moveSelector = new moveDecisionTree;
		addChild(moveSelector);
	}
};


