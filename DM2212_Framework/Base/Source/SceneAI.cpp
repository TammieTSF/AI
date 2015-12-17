#include "SceneAI.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAI::SceneAI() :
clock_rotate(0),
clock_rotate_counter(0),
currency(500),
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

// long integer to string
std::string SceneAI::itos(const long value)
{
	std::ostringstream buffer;
	buffer << value;
	return buffer.str();
}

void SceneAI::Init()
{
	SceneBase::Init();

	m_speed = 1.f;

	Math::InitRNG();

	//init empty string
	customerState = "";
	supplierState = "";

	//Intialise variables
	objectcount = 1; // Cashier is included. Total amount of objects on screen
	Females = 0;
	Males = 0;
	TotalCustomers = 0;
	RandomEnter = 0;
	//maletext = false;
	//femaletext = false;
	srand((unsigned)time(NULL));

	//Probabilities
	Gprobability = 50.0f; //gender probability
	Iprobability = 40.0f; //item probability
	Sprobability = 70.0f; //supplier probability

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Construct Cashier
	m_cashier = new GameObject(GameObject::GO_CASHIER);
	m_cashier->active = true;
	m_cashier->scale.Set(6, 6, 6);
	m_cashier->pos.Set(m_worldWidth * 0.5f, m_worldHeight * .71f, m_cashier->pos.z);

	//Construct Customer
	Customer* customer = new Customer();
	customer->setToFemale();
	customer->SetEnterState();
	m_goList.push_back(customer);

	//Construct Supplier 
	Supplier* supplier = new Supplier();
	supplier->SetEnterState();
	m_goList.push_back(supplier);

	//initialise bubblegum variables
	bubblegum.cost = 20;                // buy price
	bubblegum.max = false;
	bubblegum.no_item = 10;
	bubblegum.price = 40;               // sell price

	//initialise water variables
	water.cost = 5;                     // buy price
	water.max = false;
	water.no_item = 10;
	water.price = 10;                   // sell price

	//initialise chip variables
	chip.cost = 15;                     // buy price
	chip.max = false;
	chip.no_item = 10;
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

Customer* SceneAI::FetchCustomers()
{
	float z = 0; // initialise the z variable
	for (GameObject* GO : m_goList)
	{
		Customer* customer = dynamic_cast<Customer*>(GO);
		if (customer != NULL)
		{
			z++;
			if (customer->active == false)
			{
				RandomIndex = RandomInteger(1, 100);
				if (RandomIndex <= Gprobability) // 1- 50 = female
				{
					Females++;
					customer->setToFemale(); // it will crash if there is no if(customer != NULL)
				}
				else
				{
					Males++;
					customer->setToMale(); //51 - 100 male
				}

			}
			customer->pos.Set(customer->pos.x, customer->pos.y, z / 100);//Each time the loop runs, the z axis is increasing.
			customer->active = true;
			return customer; // BEST case scenario
		}
	}

	for (int a = 0; a < 10; a++)
	{
		m_goList.push_back(new GameObject(GameObject::GO_CUSTOMER));
	}

	Customer* customer = dynamic_cast<Customer*>(m_goList.back());
	if (customer != NULL)
	{
		customer->active = true;
		return customer; // BEST case scenario
	}
	return NULL; // worst case scenario. :D
}

Supplier* SceneAI::FetchSupplier()
{
	float z = 0; // initialise the z variable
	for (GameObject* GO : m_goList)
	{
		Supplier* supplier = dynamic_cast<Supplier*>(GO);
		if (supplier != NULL)
		{
			z++;
			supplier->pos.Set(supplier->pos.x, supplier->pos.y, z / 100);//Each time the loop runs, the z axis is increasing.
			supplier->active = true;
			return supplier; // BEST case scenario
		}
	}

	for (int a = 0; a < 10; a++)
	{
		m_goList.push_back(new GameObject(GameObject::GO_SUPPLIER));
	}

	Supplier* supplier = dynamic_cast<Supplier*>(m_goList.back());
	if (supplier != NULL)
	{
		supplier->active = true;
		return supplier; // BEST case scenario
	}
	return NULL; // worst case scenario. :D
}

bool SceneAI::getWhoEnter()
{
	RandomIndex = RandomInteger(1, 100);
	if (RandomIndex <= Sprobability) // from 0 to 30 (Supplier enters the store)
	{
		return true;
	}
	else // 31 to 100 (customer enters teh store)
	{
		return false;
	}
	
}

void SceneAI::Update(double dt)
{
	SceneBase::Update(dt);
	static float LimitCustomers = 0;

	//do update for customer/supplier movement here.
	dt *= m_speed;
	m_force.SetZero();

	//for every game object in the game object list
	for (GameObject* go : m_goList)
	{
		if (getWhoEnter() == true) //Supplier enters
		{
			//pointer to customer class is dynamic cast of go pointer
			Supplier* supplier = dynamic_cast<Supplier*>(go);

			//if customer pointer is not empty
			if (supplier != NULL)
			{
				if (GameObject::GO_SUPPLIER)
				{
					//limit the amount of customer spawn on screen at every moment
					if (LimitCustomers < 1)
					{
						//init the supplier variables
						GameObject *supplier = FetchSupplier();
						supplier->type = GameObject::GO_SUPPLIER;
						supplier->active = true;
						supplier->scale.Set(6, 6, 6);
						//left of screen
						supplier->pos.Set(0, 19, supplier->pos.z);
						supplier->vel.Set(20, 0, 0);
						LimitCustomers++;
					}
				}
			}
		}
		else //Customer enters
		{
			//pointer to customer class is dynamic cast of go pointer
			Customer* customer = dynamic_cast<Customer*>(go);

			//if customer pointer is not empty
			if (customer != NULL)
			{
				if (GameObject::GO_CUSTOMER)
				{
					//limit the amount of customer spawn on screen at every moment
					if (LimitCustomers < 1)
					{
						//init the customer variables
						GameObject *customers = FetchCustomers();
						customers->type = GameObject::GO_CUSTOMER;
						customers->active = true;
						customers->scale.Set(6, 6, 6);
						//left of screen
						customers->pos.Set(0, 19, customers->pos.z);
						customers->vel.Set(20, 0, 0);
						customer->SetEnterState();
						LimitCustomers++;
					}
				}
			}
		}
		
	}


	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;

		Customer* customer = dynamic_cast<Customer*>(go);
		Supplier* supplier = dynamic_cast<Supplier*>(go);


		//Cashier* cashier = dynamic_cast<Cashier*>(go);
		if (go->active)
		{
			if (getWhoEnter() == true) //Supplier enters
			{
				//if gameobject in m_goList is supplier 
				if (go->type == GameObject::GO_SUPPLIER)
				{
					// Makes supplier move right 
					go->pos += go->vel * dt;
					//std::cout << "supplier position x: "<<go->pos.x << ", " << "Customer pos y: " << go->pos.y << std::endl;

					if (supplier->isEnter())
					{
						//check if supplier is in the centre of the screen or entrance of the store
						if (go->pos.x > 70)
						{
							go->vel.Set(0, 20, 0);
							if (go->pos.y > 60)
							{
								supplier->SetSellGoodState();
							}
						}
					}

					if (supplier->isSelling())
					{
						
						if (RandomIndex <= Iprobability && bubblegum.max == false) // from 0 to 40
						{
							bubblegum.no_item += 1;
							currency -= 40;
						}
						else if (RandomIndex >= Iprobability && RandomIndex < 70 && water.max == false) // 40 to 70
						{
							water.no_item += 1;
							currency -= 10;
						}
						else if (RandomIndex >= 70 && RandomIndex <= 100 && chip.max == false) // 69 to 100
						{
							chip.no_item += 1;
							currency -= 30;
						}
						else
							supplier->SetLeaveState();

						supplier->SetLeaveState();
					}

					if (supplier->isLeave())
					{
						go->vel.Set(0, -20, 0);
						if (go->pos.y < 19)
						{
							go->vel.Set(20, 0, 0);
						}
					}
				}
			
			}
			else //Customer enters
			{
				//if gameobject in m_goList is customer 
				if (go->type == GameObject::GO_CUSTOMER)
				{
					// Makes customer move right 
					go->pos += go->vel * dt;
					//std::cout << "customer position x: "<<go->pos.x << ", " << "Customer pos y: " << go->pos.y << std::endl;

					if (customer->isEnter())
					{
						//check if customer is in the centre of the screen or entrance of the store (hard code eh tammie)
						if (go->pos.x > 70)
						{
							go->vel.Set(0, 20, 0);
							if (go->pos.y > 60)
							{
								customer->SetRequestState();
							}
						}
					}

					if (customer->isRequest())
					{
						if (RandomIndex <= Iprobability) // from 0 to 40
						{
							bubblegum.no_item -= 1;
							currency += 40;
						}
						else if (RandomIndex >= Iprobability && RandomIndex < 70) // 40 to 70
						{
							water.no_item -= 1;
							currency += 10;
						}
						else if (RandomIndex >= 70 && RandomIndex <= 100) // 69 to 100
						{
							chip.no_item -= 1;
							currency += 30;
						}

						if (bubblegum.no_item < 0 || water.no_item < 0 || chip.no_item < 0)
						{
							customer->SetLeaveState();
						}

						customer->SetLeaveState();
					}

					if (customer->isLeave())
					{
						go->vel.Set(0, -20, 0);
						if (go->pos.y < 19)
						{
							go->vel.Set(20, 0, 0);
						}
					}

					
				}
			}
			

			//if (GameObject::GO_CASHIER)
			//{
				//if(cashier->isGreet())
				//{
					//if(customer->isFemale())
					//{
						//cashier->SetGreetFemale();
						//cashier->isFemale();
						//femaletext = true;
					//}

					//if(customer->isMale())
					//{
						//cashier->SetGreetMale();
						//cashier->isMale();
						//maletext = true;
					//}
				//}
			//}

			// Render false if out of screen
			if (go->pos.x > m_worldWidth)
			{
				go->active = false;
				LimitCustomers--;
			}

			if (go->pos.y < 17)
			{
				go->active = false;
				LimitCustomers--;
			}
		}

	}

	//update the rotation of the clock hand
	clock_rotate_counter += 1;

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

	case GameObject::GO_SUPPLIER:
		// Render Supplier
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SUPPLIER], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_CUSTOMER:
		for (GameObject* go : m_goList)
		{
			Customer* customer = dynamic_cast<Customer*>(go);
			if (customer != NULL)
			{
				if (customer->isFemale())
				{
					// RenderMesh = GEO_FEMALE
					modelStack.PushMatrix();
					modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
					modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
					RenderMesh(meshList[GEO_FEMALE], false);
					modelStack.PopMatrix();
				}
				else if (customer->isMale())
				{
					// RenderMesh = GEO_MALE
					modelStack.PushMatrix();
					modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
					modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
					RenderMesh(meshList[GEO_MALE], false);
					modelStack.PopMatrix();
				}

			}
		}
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
	modelStack.Scale(15, 15, 1);
	RenderMesh(meshList[GEO_CLOCK_BASE], false);
	modelStack.PopMatrix();

	//render the clock hand
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth*0.9f, m_worldHeight*0.078f, -3);
	modelStack.Scale(15, 15, 1);
	modelStack.Rotate(clock_rotate, 0, 0, 1);
	RenderMesh(meshList[GEO_CLOCK_HAND], false);
	modelStack.PopMatrix();

	//render the characters for showing purpose
	
	//cashier
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth*0.05f, m_worldHeight*0.9f, -2);
	modelStack.Scale(5, 5, 1);
	RenderMesh(meshList[GEO_CASHIER], false);
	modelStack.PopMatrix();

	//supplier
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth*0.15f, m_worldHeight*0.9f, -2);
	modelStack.Scale(5, 5, 1);
	RenderMesh(meshList[GEO_SUPPLIER], false);
	modelStack.PopMatrix();

	//customer
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth*0.25f, m_worldHeight*0.9f, -2);
	modelStack.Scale(5, 5, 1);
	RenderMesh(meshList[GEO_MALE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth*0.35f, m_worldHeight*0.9f, -2);
	modelStack.Scale(5, 5, 1);
	RenderMesh(meshList[GEO_FEMALE], false);
	modelStack.PopMatrix();

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		Customer* customer = dynamic_cast<Customer*>(go);

		if (go->active)
		{
			RenderGO(go);
		}
	}

	for (GameObject* go : m_goList)
	{
		Customer* customer = dynamic_cast<Customer*>(go);
		if (customer != NULL)
		{
			switch (customer->state)
			{
			case Customer::s_ENTER:
				RenderTextOnScreen(meshList[GEO_TEXT], "customer Enter", Color(1, 1, 1), 2, 60, 53);
				break;
			case Customer::s_LEAVE:
				RenderTextOnScreen(meshList[GEO_TEXT], "customer Leave", Color(1, 1, 1), 2, 60, 53);
				break;
			case Customer::s_REQUEST:
				RenderTextOnScreen(meshList[GEO_TEXT], "customer Request", Color(1, 1, 1), 2, 60, 53);
				break;
			}

			switch (customer->gender)
			{
			case Customer::g_FEMALE:
				RenderTextOnScreen(meshList[GEO_TEXT], "customer Female", Color(1, 1, 1), 2, 60, 51);
				break;
			case Customer::g_MALE:
				RenderTextOnScreen(meshList[GEO_TEXT], "customer Male", Color(1, 1, 1), 2, 60, 51);
				break;

			}
		}

		Supplier* supplier = dynamic_cast<Supplier*>(go);
		if (supplier != NULL)
		{
			switch (supplier->state)
			{
			case Supplier::s_ENTER:
				RenderTextOnScreen(meshList[GEO_TEXT], "supplier Enter", Color(1, 1, 1), 2, 60, 55);
				break;
			case Supplier::s_LEAVE:
				RenderTextOnScreen(meshList[GEO_TEXT], "supplier Leave", Color(1, 1, 1), 2, 60, 55);
				break;
			case Supplier::s_SELLGOOD:
				RenderTextOnScreen(meshList[GEO_TEXT], "supplier Sell Goods", Color(1, 1, 1), 2, 60, 55);
				break;
			}
		}


	}

	//Render cashier
	RenderGO(m_cashier);

	//if(femaletext == true)
	//{
		//RenderTextOnScreen(meshList[GEO_TEXT], "Hello Miss.", Color(1, 0, 0), 3, 0, 57);
	//}
	//if(maletext == true)
	//{
		//RenderTextOnScreen(meshList[GEO_TEXT], "Hello Sir.", Color(1, 0, 0), 3, 0, 57);
	//}
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
	N_days << "Number of Days passed: " << day.no_Days;
	RenderTextOnScreen(meshList[GEO_TEXT], N_days.str(), Color(1, 1, 1), 2, m_worldWidth * 0.02f, m_worldHeight * 0.57f);

	//Currency
	std::ostringstream N_Currency;
	N_Currency << "$$$: " << currency;
	RenderTextOnScreen(meshList[GEO_TEXT], N_Currency.str(), Color(1, 1, 1), 2, m_worldWidth * 0.02f, m_worldHeight * 0.07f);

	//Bubblegum
	std::ostringstream N_Bubblegum;
	N_Bubblegum << "Bubblegum: " << bubblegum.no_item << " /10";
	RenderTextOnScreen(meshList[GEO_TEXT], N_Bubblegum.str(), Color(1, 1, 1), 2, m_worldWidth * 0.02f, m_worldHeight * 0.05f);

	//water
	std::ostringstream N_Water;
	N_Water << "Water: " << water.no_item << " /10";
	RenderTextOnScreen(meshList[GEO_TEXT], N_Water.str(), Color(1, 1, 1), 2, m_worldWidth * 0.02f, m_worldHeight * 0.03f);

	//chip
	std::ostringstream N_Chip;
	N_Chip << "Chip: " << chip.no_item << " /10";
	RenderTextOnScreen(meshList[GEO_TEXT], N_Chip.str(), Color(1, 1, 1), 2, m_worldWidth * 0.02f, m_worldHeight * 0.01f);

	//Get customer state
	/*Customer* customer = dynamic_cast<Customer*>(go);
	std::ostringstream Customer_State;
	Customer_State << "CU_State: " << customer->getState;
	RenderTextOnScreen(meshList[GEO_TEXT], Customer_State.str(), Color(1, 1, 1), 2, m_worldWidth * 0.44f, m_worldHeight * 0.52f);*/

	//Get Supplier state
	//Supplier* supplier_state = new Supplier();
	//std::ostringstream supplier_state;
	//supplier_state << "SUPPLIER: " << supplier_state->getStateName(Supplier::STATE::s_LEAVE);
	//RenderTextOnScreen(meshList[GEO_TEXT], supplier_state.str(), Color(1, 1, 1), 2, m_worldWidth * 0.44f, m_worldHeight * 0.52f);

	//FPS
	std::ostringstream T_fps;
	T_fps.precision(5);
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
