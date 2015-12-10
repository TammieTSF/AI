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

		GO_CUSTOMER,
		GO_CASHIER,
		GO_SUPPLIER,

		GO_TOTAL, //must be last
	};

public:
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active;
	float mass;
	int health;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	virtual ~GameObject(); // must be a virtual to handle all the children classes
	// Always inherit from parent classes, do not inherit from child classes.
};

#endif