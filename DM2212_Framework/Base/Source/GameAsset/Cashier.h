#pragma once
#include "../GameObject.h"

class Cashier :
	public GameObject
{
public:
	enum STATE
	{
		s_BUY,
		s_GREET,
	};

	enum GREET
	{
		g_FEMALE,
		g_MALE,
	};


	Cashier();
	~Cashier();
	
	STATE state;
	GREET greet;

	void SetGreetState();
	bool isGreet();

	void SetBuyState();
	bool isBuy();

	void SetGreetFemale();
	bool isFemale();

	void SetGreetMale();
	bool isMale();
};

