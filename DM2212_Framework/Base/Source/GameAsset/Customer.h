#pragma once
#include "../GameObject.h"

class Customer : public GameObject
{
public:
	enum GENDER
	{
		g_MALE, // done
		g_FEMALE, // done
	};

	enum STATE
	{
		s_ENTER,// done
		s_REQUEST, // done
		s_LEAVE, // done
	};

	Customer();
	~Customer();

	GENDER gender;

	STATE state;

	// Gender Setting
	void setToMale(); // set gender to male
	void setToFemale(); // set gender to female
	bool isMale(); // get the male gender
	bool isFemale(); // get the female gender

	// States Setting
	void SetEnterState();
	void SetRequestState();
	void SetLeaveState();

	bool isEnter();
	bool isRequest();
	bool isLeave();
};

