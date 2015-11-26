#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_ASTEROID, //Smaller Asteroid
		GO_ASTEROID2, //Larger Asteroid
		GO_SHIP,
		GO_BULLET,
		GO_MISSILE,
		GO_ENEMY,
		GO_ONEUP, // Health Regen
		GO_BOSS,
		GO_TOTAL, //must be last
	};

	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active;
	float mass;
	int health;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif