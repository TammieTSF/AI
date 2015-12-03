#include "SceneAI.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAI::SceneAI() :
clock_rotate(0),
clock_rotate_counter(0),
currency(1000),
randomBuy_no(0)
{
}

SceneAI::~SceneAI()
{
}

int SceneAI::RandomInteger(int lowerLimit, int upperLimit)
{
	return rand() % (upperLimit - lowerLimit + 1) + lowerLimit;
}

void SceneAI::Init()
{
	SceneBase::Init();
	
	m_speed = 1.f;

	Math::InitRNG();


	//Construct 100 GameObject with type GO_CUSTOMER and add into m_goList
	for (int a = 0; a < 100; a++)
	{
		m_goList.push_back(new GameObject(GameObject::GO_CUSTOMER));
	}

	//Intialise variables
	objectcount = 1; // Cashier is included. Total amount of objects on screen
	Females = 0;
	Males = 0;
	Gprobability = 50.0f;
	TotalCustomers = 0;
	srand((unsigned)time(NULL));
	
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
	//Construct Cashier
	m_cashier = new GameObject(GameObject::GO_CASHIER);
	m_cashier->active = true;
	m_cashier->scale.Set(6, 6, 6);
	m_cashier->pos.Set(m_worldWidth * 0.5f, m_worldHeight * .71f, m_cashier->pos.z);

	//initialise bubblegum variables
	bubblegum.cost = 20;                // buy price
	bubblegum.max = false;
	bubblegum.no_item = 0;
	bubblegum.price = 40;               // sell price

	//initialise water variables
	water.cost = 5;                     // buy price
	water.max = false;
	water.no_item = 0;
	water.price = 10;                   // sell price

	//initialise chip variables
	chip.cost = 15;                     // buy price
	chip.max = false;
	chip.no_item = 0;
	chip.price = 30;                    // sell price

	//initialiise the day vaiables
	day.no_Days = 0;
	day.day_State = 0;
}

GameObject* SceneAI::FetchGO()
{
	//Fetch game objects
	float z = 0; //Set z as zero
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it, ++z) //post-increment z
	{
		GameObject *go = (GameObject *)*it;
		if (go->active == false)
		{
			RandomIndex = RandomInteger(1, 100);
			if (RandomIndex <= Gprobability)
			{
				Females++;
				CGender = GENDER_FEMALE;
			}
			else
			{
				Males++;
				CGender = GENDER_MALE;
			}

			go->pos.Set(go->pos.x, go->pos.y, z / 100);//Each time the loop runs, the z axis is increasing.
			go->active = true;
			return go;
		}
	}
	//Restock the list when m_goList runs out of object
	for (int a = 0; a < 10; a++)
	{
		m_goList.push_back(new GameObject(GameObject::GO_CUSTOMER));
	}

	GameObject *go = m_goList.back();
	go->active = true;
	return go;
}

void SceneAI::Update(double dt)
{
	SceneBase::Update(dt);

	//do update for customer/supplier movement here.
	dt *= m_speed;
	m_force.SetZero();

	//update the rotation of the clock hand
	clock_rotate_counter +=1;

	if (clock_rotate_counter > 60)
	{
		clock_rotate -= 360 / 12;
		clock_rotate_counter = 0;
	}
	
	//reset the clock, increase no. of days by 1
	if (clock_rotate < -360)
	{
		clock_rotate = 0;
		day.no_Days++;
	}
		

	//item limit max. 10
	
	//bubblegum
	if (bubblegum.no_item == 10)
		bubblegum.max = true;
	else
		bubblegum.max = false;

	//water
	if (water.no_item == 10)
		water.max = true;
	else
		water.max = false;

	//chip
	if (chip.no_item == 10)
		chip.max = true;
	else
		chip.max = false;


	static float LimitCustomers = 0;
	if (GameObject::GO_CUSTOMER)
	{
		if (LimitCustomers < 1)
		{
			GameObject *customers = FetchGO();
			customers->type = GameObject::GO_CUSTOMER;
			customers->scale.Set(6, 6, 6);
			customers->pos.Set(0, 0, 0);
			customers->vel.Set(0, 0, 0);

			LimitCustomers++;
		}
	}

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;

		if (go->active)
		{
			// Makes objects move
			go->pos += go->vel * dt;

			// Render customer false if out of screen
			if (go->type == GameObject::GO_CUSTOMER)
			{
				if (go->pos.x > m_worldWidth)
				{
					go->active = false;
				}
			}
		}
	}
}

void SceneAI::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_CASHIER:
		// Render Cashier
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CASHIER], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_CUSTOMER:
		switch (CGender)
		{
		case GENDER_FEMALE:
			// RenderMesh = GEO_FEMALE
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_FEMALE], false);
			modelStack.PopMatrix();
			break;

		case GENDER_MALE:
			// RenderMesh = GEO_MALE
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_MALE], false);
			modelStack.PopMatrix();
			break;
		}
		break;
	
	case GameObject::GO_SUPPLIER:
		//RenderMesh = GEO_SUPPLIER
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SUPPLIER], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneAI::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();


	//render the background image
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth*0.5f, m_worldHeight*0.425f, -5);
	modelStack.Scale(m_worldWidth, m_worldHeight, 1);
	RenderMesh(meshList[GEO_BG], false);
	modelStack.PopMatrix();

	//render the clock base
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth*0.9f, m_worldHeight*0.078f, -4);
	modelStack.Scale(15,15,1);
	RenderMesh(meshList[GEO_CLOCK_BASE], false);
	modelStack.PopMatrix();

	//render the clock hand
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth*0.9f, m_worldHeight*0.078f, -3);
	modelStack.Scale(15,15,1);
	modelStack.Rotate(clock_rotate,0,0,1);
	RenderMesh(meshList[GEO_CLOCK_HAND], false);
	modelStack.PopMatrix();


	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}


	//Render cashier
	RenderGO(m_cashier);

	//On screen text

	/*if (m_score == 0 || m_score < 100)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Level 1", Color(1, 0, 0), 3, 0, 57);
	}
	else if (m_score > 100 && m_score < 300)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Level 2", Color(1, 1, 0), 3, 0, 57);
	}
	else if (m_score >= 300)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Level 3", Color(1, 0, 1), 3, 0, 57);
	}*/

	//DAYS
	std::ostringstream N_days;
	N_days.precision(5);
	N_days << "Number of Days passed: " << day.no_Days;
	RenderTextOnScreen(meshList[GEO_TEXT], N_days.str(), Color(1, 1, 1), 2, m_worldWidth * 0.02f,m_worldHeight * 0.57f);

	//Currency
	std::ostringstream N_Currency;
	N_Currency.precision(5);
	N_Currency << "$$$: " << currency;
	RenderTextOnScreen(meshList[GEO_TEXT], N_Currency.str(), Color(1, 1, 1), 2, m_worldWidth * 0.02f, m_worldHeight * 0.07f);

	//Bubblegum
	std::ostringstream N_Bubblegum;
	N_Bubblegum.precision(5);
	N_Bubblegum << "Bubblegum: " << bubblegum.no_item << " /10" ;
	RenderTextOnScreen(meshList[GEO_TEXT], N_Bubblegum.str(), Color(1, 1, 1), 2, m_worldWidth * 0.02f, m_worldHeight * 0.05f);

	//water
	std::ostringstream N_Water;
	N_Water.precision(5);
	N_Water << "Water: " << water.no_item << " /10";
	RenderTextOnScreen(meshList[GEO_TEXT], N_Water.str(), Color(1, 1, 1), 2, m_worldWidth * 0.02f, m_worldHeight * 0.03f);

	//chip
	std::ostringstream N_Chip;
	N_Chip.precision(5);
	N_Chip << "Chip: " << chip.no_item << " /10";
	RenderTextOnScreen(meshList[GEO_TEXT], N_Chip.str(), Color(1, 1, 1), 2, m_worldWidth * 0.02f, m_worldHeight * 0.01f);

	//FPS
	std::ostringstream T_fps;
	T_fps << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], T_fps.str(), Color(1, 1, 1), 2, m_worldWidth * .44f, m_worldHeight * 0.57f);


}

void SceneAI::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if (m_cashier)
	{
		delete m_cashier;
		m_cashier = NULL;
	}
}
