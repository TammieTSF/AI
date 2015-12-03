#ifndef SCENE_SCENEAI_H
#define SCENE_SCENEAI_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneAI : public SceneBase
{
	static const int MAX_SPEED = 10;
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;

	struct MyVector
	{
		float x, y;
		MyVector() :x(0), y(0){}
		MyVector(float x, float y) :x(x), y(y){}
		void SetPosition(float _x, float _y){ x = _x; y = _y; }
		float GetX(){ return x; }
		float GetY(){ return y; }
		float Magnitude(){ return sqrt(x*x + y*y); }
		MyVector Normalize(){ float length = Magnitude(); return MyVector(x / length, y / length); }
		MyVector operator + (MyVector u){ return MyVector(x + u.x, y + u.y); }
		MyVector operator - (MyVector u){ return MyVector(u.x - x, u.y - y); }
		MyVector operator += (MyVector u){ return MyVector(x + u.x, y + u.y); }
		MyVector operator ~(){ return MyVector(-x, -y); }
		MyVector operator *(float scale){ return MyVector(x*scale, y*scale); }
		float operator * (MyVector  v){ return  x*v.x + y*v.y; }
	};

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
	enum CUSTOMER
	{
		// Gender States, probability based
		GENDER_FEMALE = 0,
		GENDER_MALE,

		// Game States
		CUSTOMER_ENTER,
		CUSTOMER_REQUEST,
		CUSTOMER_LEAVE,
		CUSTOMER_BARGAIN,

		// Customer Enter Shop states, probability based
		ENTER_TRUE,
		ENTER_FALSE,

		// Recommend states, for when cashier recommends, probability based
		RECOMMEND_ACCEPT,
		RECOMMEND_REFUSE,

		CUSTOMER_DEFAULT
	};

	enum CASHIER
	{
		// Game States
		CASHIER_GREET = 0,
		CASHIER_RECOMMEND,

		// Greeting States, probability state
		GREET_FEMALE,
		GREET_MALE,

		// Bargain States, for when customer bargains, probability state
		BARGAIN_ACCEPT,
		BARGAIN_REFUSE,

		CASHIER_DEFAULT
	};

	enum SUPPLIER
	{
		// Game States
		SUPPLIER_SUPPlY = 0,
		SUPPLIER_ENTER,
		SUPPLIER_LEAVE,

		SUPPLIER_DEFAULT
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
	int CGender; // Customer's Gender
	int Females; // No. of females customers
	int Males; // No. of male customers
	int RandomIndex; // Randomize Integers
	int TotalCustomers; // Total amount of customers
	int wayPointIndex;

	float Gprobability; // Gender probability
	float Eprobability; // Enter shop probability

	const float CustomerSpeed = 0.0275f;
	const float SupplierSpeed = 0.0275f;
	const float CustomerRadius = 0.2f;
	const float SupplierRadius = 0.2f;
	const float proximity = 0.4f; // proximity between supplier and waypoints/ proximity between customer and cashier

	bool arrived;

	MyVector CustomerPos, SupplierPos;
	std::vector<MyVector>wayPoints, intrusionPoints;
	std::vector<MyVector> stack;

};

#endif