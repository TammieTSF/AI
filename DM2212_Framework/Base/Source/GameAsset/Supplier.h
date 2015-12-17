#pragma once
#include "../GameObject.h"
#include <string.h>

class Supplier : public GameObject
{
	
public:
	enum STATE
	{
		s_ENTER,
		s_SELLGOOD,
		s_LEAVE,
	};

	Supplier();
	~Supplier();

	STATE state;

	// States Setting
	void SetEnterState();
	void SetSellGoodState();
	void SetLeaveState();

	bool isEnter();
	bool isSelling();
	bool isLeave();

	int getState();
	std::string getStateName(STATE state);
};

