#ifndef SCENE_SCENEAI_H
#define SCENE_SCENEAI_H

#include "GameObject.h"
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <string>
#include <ostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFw/glfw3.h>
#include "SceneBase.h"
#include "GameAsset\Customer.h"
#include "GameAsset\Cashier.h"

class SceneAI : public SceneBase
{
	struct Item
	{
		int no_item;
		int cost; //cost price (buy from supplier)
		int price; //selling price (sell to customer)
		bool max;
	};

	struct Day
	{
		int no_Days;
		int day_State;
	};

public:
	SceneAI();
	~SceneAI();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
	Customer* FetchCustomers();

	int RandomInteger(int lowerLimit, int upperLimit);

protected:

	//Physics
	std::vector<GameObject *> m_goList;

	float m_speed;
	float m_worldWidth;
	float m_worldHeight;

	//for clock
	float clock_rotate;
	float clock_rotate_counter;

	//for No. of days
	Day day;

	//currency
	int currency;

	//inventory /items
	Item bubblegum;
	Item water;
	Item chip;

	int randomBuy_no;

	GameObject *m_cashier;

	Vector3 m_force;

	int m_objectCount;
	int objectcount;

	//Variables
	int Females;		// No. of females customers
	int Males;			// No. of male customers
	int TotalCustomers; // Total amount of customers
	int RandomIndex;	// Randomize Integers

	//Probability
	float Gprobability; // Gender probability
	float Iprobability; // Item buy probability

	//Render Text
	//bool maletext; // Render text for males
	//bool femaletext; // Render text for females
};

#endif