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
	float GetDistance(float x1, float y1, float x2, float y2);
	bool Detect(MyVector pos1, MyVector pos2, float radius1, float radius2);
	void RenderCircle(GLfloat x, GLfloat y, GLfloat radius, GLfloat r, GLfloat g, GLfloat b);
	void RenderFillCircle(GLfloat x, GLfloat y, GLfloat radius, GLfloat r, GLfloat g, GLfloat b);

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
	int Enter;			// No. of people entered
	int NoEnter;		// No. of people who didn't enter shop
	int TotalEnters;	// Total amount of people who entered 
	int RandomIndex;	// Randomize Integers
	int wayPointIndex;
	int desiredItem;

	//States
	int CGender;	// Customer's Gender
	int CState;		// Customer state
	int EState;		// Enter state
	int BState;		// Bargain state
	int RState;		// Recommend state
	int cState;		// Cashier state
	int GState;		// Greet State

	//Probability
	float Gprobability; // Gender probability
	float Eprobability; // Enter shop probability
	float Rprobability; // Recommend probability
	float Bprobability; // Bargain probability

	//Speed, Radius, etc.
	const float CustomerSpeed = 10.f;
	const float SupplierSpeed = 0.0275f;
	const float tempspd = 0.0275f;
	const float CustomerRadius = 0.2f;
	const float SupplierRadius = 0.2f;
	const float temprad = 0.2f;
	const float proximity = 0.4f; // proximity between supplier and waypoints/ proximity between customer and cashier

	bool arrived;

	MyVector CustomerPos, SupplierPos, Temp;
	std::vector<MyVector>wayPoints, intrusionPoints;
	MyVector nextPoint; //Go to next point
	std::vector<MyVector> stack;

};

#endif