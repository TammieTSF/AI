#include "SceneAsteroid.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAsteroid::SceneAsteroid()
{
}

SceneAsteroid::~SceneAsteroid()
{
}

void SceneAsteroid::Init()
{
	SceneBase::Init();

	showInfo = false;
	StartGame = false;
	GameOver = false;
	Win = false;
	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	//Construct 100 GameObject with type GO_ASTEROID and add into m_goList
	for(int a = 0; a < 100; a++)
	{
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
	}

	//Intialise m_lives, m_score
	m_lives = 10;
	m_score = 0;
	objectcount = 1; // objectcount equal to 1 because player ship is included.

	//Construct m_ship
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_ship = new GameObject(GameObject::GO_SHIP);
	m_ship->active = true;
	m_ship->scale.Set(8, 8, 8);
	m_ship->pos.Set(m_worldWidth*0.5f, m_worldHeight*0.5f, m_ship->pos.z);
}

GameObject* SceneAsteroid::FetchGO()
{
	//Fetch game objects
	float z = 0; //Set z as zero
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it, ++z) //post-increment z
	{
		GameObject *go = (GameObject *)*it;
		if(go->active == false)
		{
			go->pos.Set(go->pos.x, go->pos.y, z/100);//Each time the loop runs, the z axis is increasing.
			go->active = true;
			return go;
		}
	}
	//Restock the list when m_goList runs out of object
	for(int a = 0; a < 10; a++)
	{
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
	}

	GameObject *go = m_goList.back();
	go->active = true;
	return go;
}

void SceneAsteroid::Update(double dt)
{
	if(StartGame == true)
	{
		SceneBase::Update(dt);

		//Speed configuration
		if(Application::IsKeyPressed('9'))
		{
			m_speed = Math::Max(0.f, m_speed - 0.1f);

		}
		if(Application::IsKeyPressed('0'))
		{
			m_speed += 0.1f;
		}
		dt *= m_speed;
		m_force.SetZero();

		//Ship movements
		if(Application::IsKeyPressed('W'))
		{
			m_force.y = 100.f;
		}
		if(Application::IsKeyPressed('A'))
		{
			m_force.x = -100.f;
		}
		if(Application::IsKeyPressed('S'))
		{
			m_force.y = -100.f;
		}
		if(Application::IsKeyPressed('D'))
		{
			m_force.x = 100.f;
		}

		//Mass configuration
		if(Application::IsKeyPressed(VK_UP))
		{
			m_ship->mass += 0.1f;
		}
		if(Application::IsKeyPressed(VK_DOWN))
		{
			m_ship->mass -= 0.1f;

			if(m_ship->mass < 0.1f)
			{
				m_ship->mass = 0.1f;
			}
		}

		//Configure Game Info Settings

		if(Application::IsKeyPressed('P'))
		{
			showInfo = true;
		}
		else if(Application::IsKeyPressed('O'))
		{
			showInfo = false;
		}


		//For spawning GO_ASTEROID and GO_ASTEROID2
		static float limita = 0;
		if(GameObject::GO_ASTEROID || GameObject::GO_ASTEROID)
		{
			if(m_score < 100) //Level 1
			{
				if (limita < 10)
				{
					GameObject *asteroids = FetchGO();
					asteroids->type = GameObject::GO_ASTEROID;
					asteroids->scale.Set(3, 3, 3);
					asteroids->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, asteroids->pos.z);
					if(asteroids->pos == m_ship->pos)
					{
						asteroids->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, asteroids->pos.z);
					}
					asteroids->vel.Set((rand() % 11) - 5, (rand() % 11) - 5, 0);
					objectcount++;
					limita++;
				}
			}
			else if(m_score > 100 && m_score < 300) //Level 2
			{
				if(limita < 15)
				{
					GameObject *asteroids = FetchGO();
					asteroids->type = GameObject::GO_ASTEROID;
					asteroids->scale.Set(3, 3, 3);
					asteroids->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, asteroids->pos.z);
					if(asteroids->pos == m_ship->pos)
					{
						asteroids->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, asteroids->pos.z);
					}
					asteroids->vel.Set((rand() % 11) - 10, (rand() % 11) - 10, 0);
					objectcount++;
					limita++;

					GameObject *asteroids2 = FetchGO();
					asteroids2->type = GameObject::GO_ASTEROID2;
					asteroids2->health = 3;
					asteroids2->scale.Set(7, 7, 7);
					asteroids2->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, asteroids2->pos.z);
					if(asteroids2->pos == m_ship->pos)
					{
						asteroids2->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, asteroids->pos.z);
					}
					asteroids2->vel.Set((rand() % 11) - 10, (rand() % 11) - 10, 0);
					objectcount++;
					limita++;
				}
			}
			else if(m_score > 300) //Level 3
			{
				if(limita < 20)
				{
					GameObject *asteroids = FetchGO();
					asteroids->type = GameObject::GO_ASTEROID;
					asteroids->scale.Set(3, 3, 3);
					asteroids->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, asteroids->pos.z);
					if(asteroids->pos == m_ship->pos)
					{
						asteroids->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, asteroids->pos.z);
					}
					asteroids->vel.Set((rand() % 11) - 15, (rand() % 11) - 15, 0);
					objectcount++;
					limita++;

					GameObject *asteroids2 = FetchGO();
					asteroids2->type = GameObject::GO_ASTEROID2;
					asteroids2->health = 3;
					asteroids2->scale.Set(7, 7, 7);
					asteroids2->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, asteroids2->pos.z);
					if(asteroids2->pos == m_ship->pos)
					{
						asteroids2->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, asteroids->pos.z);
					}
					asteroids2->vel.Set((rand() % 11) - 12, (rand() % 11) - 12, 0);
					objectcount++;
					limita++;
				}
			}
		}


		//For spawning GO_BULLET
		static float timeget;
		static float timeLimit = 0.2f; // 0.1f spawns 10 bullets
		timeget += dt;

		if (timeget > timeLimit)
		{
			timeget = timeLimit;
		}

		if (timeget >= timeLimit)
		{	
			GameObject *bullets = FetchGO();
			bullets->type = GameObject::GO_BULLET;
			bullets->scale.Set(0.3, 0.3, 0.3);
			bullets->pos.Set(m_ship->pos.x, m_ship->pos.y, m_ship->pos.z);
			if(m_ship->vel.IsZero())
			{
				//Must use default value, shoot bullet to right.
				bullets->vel.Set(m_ship->pos.x, 0, 0);
			}
			else
			{
				//Must normalize velocity.
				bullets->vel.Set(m_ship->vel.x, m_ship->vel.y, 0);	
				bullets->vel.Normalize() *= BULLET_SPEED;
			}
			bullets->active = true;
			timeget = 0;
		}



		//For spawning GO_ENEMY
		static float limit = 0;
		if(GameObject::GO_ENEMY)
		{
			if (limit < 1)
			{
				GameObject *enemy = FetchGO();
				enemy->type = GameObject::GO_ENEMY;
				enemy->scale.Set(8, 8, 8);
				enemy->pos.Set(5, m_worldHeight - 10, enemy->pos.z);
				enemy->vel.Set((rand() % 11) + 15, 0, 0);
				enemy->active = true;
				objectcount++;
				limit++;
			}
		}


		//For spawning GO_BOSS
		static float Blimit = 0;
		if(GameObject::GO_BOSS)
		{
			if (m_score > 300 && Blimit < 1)
			{
				GameObject *boss = FetchGO();
				boss->type = GameObject::GO_BOSS;
				boss->scale.Set(20, 20, 20);
				boss->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, boss->pos.z);
				if(boss->pos == m_ship->pos)
				{
					boss->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, boss->pos.z);
				}
				boss->vel.Set((rand() % 11) - 20, (rand() % 11) - 20, 0);
				boss->health = 50;
				boss->active = true;
				objectcount++;
				Blimit++;
			}
		}


		//For spawning GO_ONEUP
		static float limitH = 0;

		if(GameObject::GO_ONEUP)
		{
			if (m_lives < 6 && limitH < 1)
			{
				GameObject *oneup = FetchGO();
				oneup->type = GameObject::GO_ONEUP;
				oneup->scale.Set(5, 5, 5);
				oneup->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, oneup->pos.z);
				oneup->vel.Set((rand() % 11) - 5, (rand() % 11) - 5, 0);
				oneup->active = true;
				objectcount++;
				limitH++;
			}
		}


		//Mouse Section
		static bool bLButtonState = false;
		if(!bLButtonState && Application::IsMousePressed(0))
		{
			bLButtonState = true;
			std::cout << "LBUTTON DOWN" << std::endl;
		}
		else if(bLButtonState && !Application::IsMousePressed(0))
		{
			bLButtonState = false;
			std::cout << "LBUTTON UP" << std::endl;
		}
		static bool bRButtonState = false;
		if(!bRButtonState && Application::IsMousePressed(1))
		{
			bRButtonState = true;
			std::cout << "RBUTTON DOWN" << std::endl;
		}
		else if(bRButtonState && !Application::IsMousePressed(1))
		{
			bRButtonState = false;
			std::cout << "RBUTTON UP" << std::endl;
		}

		//Physics Simulation Section

		//Update ship's velocity based on m_force
		//F = m * a
		//a = F / m

		Vector3 acc = m_force * (1.f / m_ship->mass);
		m_ship->vel += acc * dt;
		m_ship->pos += m_ship->vel * dt;

		//Velocity Limit
		if(m_ship->vel.Length() > MAX_SPEED)
		{
			m_ship->vel.Normalize() *= MAX_SPEED;
		}

		//Wraps the ship if it leaves screen
		if(m_ship->pos.x > m_worldWidth)
		{
			m_ship->pos.x = 0;
		}
		else if(m_ship->pos.x < 0)
		{
			m_ship->pos.x = m_worldWidth;
		}

		if(m_ship->pos.y > m_worldHeight)
		{
			m_ship->pos.y = 0;
		}
		else if(m_ship->pos.y < 0)
		{
			m_ship->pos.y = m_worldHeight;
		}

		for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;

			if(go->active)
			{
				//GO_SHIP and GO_ASTEROID collision
				if(go->type == GameObject::GO_ASTEROID)
				{
					if((m_ship->pos - go->pos).Length() < ((m_ship->scale.x - 6) + go->scale.x))
					{
						m_ship->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, m_ship->pos.z);
						m_ship->vel.Set(0,0,0);
						m_lives--;
						objectcount--;
						limita--;
						go->active = false;
						if(m_lives <= 0)
						{
							GameOver = true;
							StartGame = false;
						}
					}	
				}

				//GO_SHIP and GO_ASTEROID2 collision
				if(go->type == GameObject::GO_ASTEROID2)
				{
					if((m_ship->pos - go->pos).Length() < ((m_ship->scale.x - 8) + go->scale.x))
					{
						m_ship->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, m_ship->pos.z);
						m_ship->vel.Set(0,0,0);
						m_lives--;
						objectcount--;
						limita--;
						go->active = false;			
						if(m_lives <= 0)
						{
							GameOver = true;
							StartGame = false;
						}
					}
				}

				//GO_SHIP and GO_ENEMY collision
				if(go->type == GameObject::GO_ENEMY)
				{
					if((m_ship->pos - go->pos).Length() < ((m_ship->scale.x - 8) + go->scale.x))
					{
						m_ship->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, m_ship->pos.z);
						m_ship->vel.Set(0,0,0);
						m_lives--;
						objectcount--;
						limit--;
						go->active = false;
						if(m_lives <= 0)
						{
							GameOver = true;
							StartGame = false;
						}
					}
				}


				//GO_SHIP and GO_BOSS collision
				if(go->type == GameObject::GO_BOSS)
				{
					if((m_ship->pos - go->pos).Length() < ((m_ship->scale.x - 15) + go->scale.x))
					{
						go->pos.Set(rand() % (int)m_worldWidth, rand() % (int)m_worldHeight, go->pos.z);
						m_ship->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, m_ship->pos.z);
						m_ship->vel.Set(0,0,0);
						m_lives -= 2;
						if(m_lives <= 0)
						{
							GameOver = true;
							StartGame = false;
						}
					}
				}


				//GO_SHIP and GO_ONEUP collision
				if(go->type == GameObject::GO_ONEUP)
				{
					if((m_ship->pos - go->pos).Length() < ((m_ship->scale.x - 7) + go->scale.x))
					{
						m_lives++;
						limitH--;
						go->active = false;
					}
				}

				//Makes asteroids wrap around the screen
				go->pos += go->vel * dt;

				if(go->type == GameObject::GO_ASTEROID || go->type == GameObject::GO_ASTEROID2 || go->type == GameObject::GO_BOSS)
				{
					if(go->pos.x > m_worldWidth)
					{
						go->pos.x = 0;
					}
					else if(go->pos.x < 0)
					{
						go->pos.x = m_worldWidth;
					}

					if(go->pos.y > m_worldHeight)
					{
						go->pos.y = 0;
					}
					else if(go->pos.y < 0)
					{
						go->pos.y = m_worldHeight;
					}
				}

				//Makes the enemy ship go lower after moving out of m_worldWidth
				if(go->type == GameObject::GO_ENEMY)
				{
					if(go->pos.x > m_worldWidth)//checks if ship has gone out of m_worldWidth
					{
						go->pos.x = 0; 
						if(go->pos.y < m_worldHeight && go->pos.y != 0)//check if pos.y is less than m_worldHeight and not equal to zero
						{
							go->pos.y -= 10;
						}
						else
						{
							go->pos.y = m_worldHeight - 10;//set it back to original y pos.
						}
					}
				}

				//Unspawn bullets when they leave screen
				if(go->type == GameObject::GO_BULLET)
				{
					if(go->pos.x > m_worldWidth || go->pos.x < 0)
					{
						go->active = false;

					}
					if(go->pos.y > m_worldHeight || go->pos.y < 0)
					{
						go->active = false;

					}
				}

				//Unspawn health regens when they leave screen
				if(go->type == GameObject::GO_ONEUP)
				{
					if(go->pos.x > m_worldWidth || go->pos.x < 0)
					{
						limitH--;
						objectcount--;
						go->active = false;

					}
					if(go->pos.y > m_worldHeight || go->pos.y < 0)
					{
						limitH--;
						objectcount--;
						go->active = false;
					}
				}

				//GO_BULLET and GO_ASTEROID collision
				for(std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
				{
					GameObject *go2 = (GameObject *)*it2;
					if(go->type == GameObject::GO_ASTEROID && go2->type == GameObject::GO_BULLET)
					{
						if((go->pos - go2->pos).Length() < ((go->scale.x - 1) + go2->scale.x))
						{						
							go->active = false;
							m_score += 2;
							objectcount--;
							limita--;
							go2->active = false;

						}
					}
				}

				//GO_BULLET and GO_ASTEROID2 collision
				for(std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
				{
					GameObject *go2 = (GameObject *)*it2;

					if(go->type == GameObject::GO_ASTEROID2 && go2->type == GameObject::GO_BULLET)
					{
						if((go->pos - go2->pos).Length() < ((go->scale.x - 4) + go2->scale.x))
						{
							for(int a = 0; a < 2; a++)
							{
								GameObject *asteroids = FetchGO();
								asteroids->type = GameObject::GO_ASTEROID;
								asteroids->scale.Set(3, 3, 3);
								asteroids->pos.Set(go->pos.x, go->pos.y, go->pos.z + a);
								asteroids->vel.Set((rand() % 11) - 5, (rand() % 11) - 5, 0);
								limita++;
							}
							go->active = false;
							go2->active = false;
							m_score += 2;
							limita--;
							objectcount++;
						}
					}
				}

				//GO_BULLET and GO_BOSS collision
				for(std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
				{
					GameObject *go2 = (GameObject *)*it2;

					if(go->type == GameObject::GO_BOSS && go2->type == GameObject::GO_BULLET)
					{
						if((go2->pos - go->pos).Length() < (go2->scale.x + (go->scale.x- 12)))
						{
							if(go->health <= 0)
							{
								go->active = false;
								m_score += 400;
								Win = true;
								StartGame = false;
							}

							go2->active = false;
							go->health--;
						}
					}
				}
			}
		}
	}
}


void SceneAsteroid::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_SHIP:
		{
			//Render Ship
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, 10);
			float theta = Math::RadianToDegree(atan2(m_ship->vel.y, m_ship->vel.x));
			modelStack.Rotate(theta, 0, 0, 1);
			modelStack.Scale(go->scale.x,go->scale.y,go->scale.z);
			RenderMesh(meshList[GEO_SHIP], false);
			modelStack.PopMatrix();
			break;
		}

	case GameObject::GO_ASTEROID:
		{
			//Render Asteroid
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_ASTEROID], false);
			modelStack.PopMatrix();
			break;
		}

	case GameObject::GO_ASTEROID2:
		{
			//Render Asteroid2
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_ASTEROID2], false);
			modelStack.PopMatrix();
			break;
		}

	case GameObject::GO_BULLET:
		{
			//Render Bullets
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_BALL], false);
			modelStack.PopMatrix();
			break;
		}

	case GameObject::GO_ENEMY:
		//Render Enemy
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ENEMY], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_ONEUP:
		//Render Health Mushrooms
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ONEUP], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_BOSS:
		//Render Boss
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOSS], false);
		modelStack.PopMatrix();
		break;
	}
}


void SceneAsteroid::Menu()
{
	std::string text = "Welcome to Asteroid Space Shooter";

	RenderTextOnScreen(meshList[GEO_TEXT], text, Color(0, 1, 1), 3, 10, 35);

	std::string menu[2] = {"Start Game","Quit Game"};

	for(int a = 0; a < 2; a++)
	{
		if (a == Selection)
		{
			Colour = 0;
		}

		else
		{
			Colour = 1;
		}

		if(Application::IsKeyPressed(VK_DOWN)) 
		{
			Selection = 1;
		}

		if(Application::IsKeyPressed(VK_UP))
		{
			Selection = 0;
		}

		RenderTextOnScreen(meshList[GEO_TEXT], menu[a], Color(Colour, 1, Colour), 3, 31 + a, 32 - (a*2));

	}

	if(Application::IsKeyPressed(VK_RETURN))
	{
		if (Selection == 0)
		{
			StartGame = true;
		}	

		else
		{
			exit(0);
		}
	}
}

void SceneAsteroid::Render()
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




	if(StartGame == false && GameOver == false && Win == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth*0.5f, m_worldHeight*0.5f, -5);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[GEO_MENU], false);
		modelStack.PopMatrix();

		Menu();
	}

	else if(StartGame == true && GameOver == false && Win == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth*0.5f, m_worldHeight*0.5f, -5);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[GEO_BG], false);
		modelStack.PopMatrix();

		for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if(go->active)
			{
				RenderGO(go);
			}
		}

		RenderGO(m_ship);

		//On screen text
		if(m_score == 0 || m_score < 100)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Level 1", Color(1, 0, 0), 3, 0, 57);
		}
		else if(m_score > 100 && m_score < 300)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Level 2", Color(1, 1, 0), 3, 0, 57);
		}
		else if(m_score >= 300)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Level 3", Color(1, 0, 1), 3, 0, 57);
		}

		std::ostringstream ss6;
		ss6.precision(5);
		ss6 << "Lives: " << m_lives;
		RenderTextOnScreen(meshList[GEO_TEXT], ss6.str(), Color(0, 1, 1), 3, 0, 55);

		std::ostringstream ss5;
		ss5.precision(5);
		ss5 << "Score: " << m_score;
		RenderTextOnScreen(meshList[GEO_TEXT], ss5.str(), Color(0, 1, 1), 3, 16, 55);

		if(showInfo == true)
		{
			std::ostringstream ss7;
			ss7.precision(5);
			ss7<< "Object count: " << objectcount;
			RenderTextOnScreen(meshList[GEO_TEXT], ss7.str(), Color(1, 1, 1), 3, 0, 8);

			std::ostringstream ss4;
			ss4.precision(5);
			ss4 << "Pos: " << m_ship->pos; 
			RenderTextOnScreen(meshList[GEO_TEXT], ss4.str(), Color(1, 1, 1), 3, 0, 6);

			std::ostringstream ss3;
			ss3.precision(5);
			ss3 << "Vel: " << m_ship->vel;
			RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(1, 1, 1), 3, 0, 4);

			std::ostringstream ss2;
			ss2.precision(5);
			ss2 << "Mass: " << m_ship->mass;
			RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(1, 1, 1), 3, 16, 2);

			std::ostringstream ss1;
			ss1.precision(3);
			ss1 << "Speed: " << m_speed;
			RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(1, 1, 1), 3, 0, 2);

			std::ostringstream ss;
			ss.precision(5);
			ss << "FPS: " << fps;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 0);
		}
	}
	else if(GameOver == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth*0.5f, m_worldHeight*0.5f, -5);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[GEO_GAMEOVER], false);
		modelStack.PopMatrix();

		std::string text = "Game Over";
		RenderTextOnScreen(meshList[GEO_TEXT], text, Color(1, 0, 0), 10, 15, 35);

		std::ostringstream ss;
		ss.precision(5);
		ss << "Final Score: " << m_score;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 4, 20, 32);

		std::string text1 = "Press ESC to exit game.";
		RenderTextOnScreen(meshList[GEO_TEXT], text1, Color(1, 0, 0), 3, 19, 29);
	}

	else if(Win == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth*0.5f, m_worldHeight*0.5f, -5);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[GEO_GAMEOVER], false);
		modelStack.PopMatrix();

		std::string text = "You Won!! Congratulations!";
		RenderTextOnScreen(meshList[GEO_TEXT], text, Color(1, 1, 0), 5, 2, 35);

		std::ostringstream ss;
		ss.precision(5);
		ss << "Final Score: " << m_score;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 4, 20, 32);

		std::string text1 = "Press ESC to exit game.";
		RenderTextOnScreen(meshList[GEO_TEXT], text1, Color(1, 1, 0), 3, 19, 29);
	}
}

void SceneAsteroid::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ship)
	{
		delete m_ship;
		m_ship = NULL;
	}
}
