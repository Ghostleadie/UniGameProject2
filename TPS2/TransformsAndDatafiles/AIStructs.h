#pragma once

#include "TransformComponent.h"
#include "GameObject.h"


enum healthState {
	fatal,
	hurt,
	healthy
};

enum AIType {
	stealthy,
	aggressive,
	balanced
};

enum AIState {
	roaming,
	fighting,
	alert
};

struct perceptionData {
	GameObject* currentTarget;
	vector<GameObject*> v_go;
};

struct rangedWeaponData {
	int ammoCount = 5;
	int magSize = 5;
	float reloadTimer = 3;
	float reloadTime = 3;
	float fireDelay = 1;
	float delayTimer = 0;
	bool canShoot = true;
	bool reloading = false;
};

struct meleeWeaponData {
	float attackDelay = 0.5;
	float delayTimer = 0.5;
};

struct AIData {
	GameObject* ai;
	AIType type;
	AIState state;
	healthState healthState;
	int currentHealth;
	int lives;

	TransformComponent* tComp;

	perceptionData pData;
	rangedWeaponData wData;
	meleeWeaponData mData;
};
