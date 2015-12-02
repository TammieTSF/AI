#include "SceneAsteroid.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

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
	
	m_speed = 1.f;

	Math::InitRNG();

	//Construct 100 GameObject with type GO_ASTEROID and add into m_goList
	for (int a = 0; a < 100; a++)
	{
		//m_goList.push_back(new GameObject(GameObject::GO_CUSTOMER));
	}

	//Intialise m_lives, m_score
	m_lives = 10;
	m_score = 0;
	objectcount = 1; // objectcount equal to 1 because player ship is included.

	//Construct Cashier
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_cashier = new GameObject(GameObject::GO_CASHIER);
	m_cashier->active = true;
	m_cashier->scale.Set(6, 6, 6);
	m_cashier->pos.Set(70, 80, m_cashier->pos.z);
}

GameObject* SceneAsteroid::FetchGO()
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
		//m_goList.push_back(new GameObject(GameObject::GO_CUSTOMER));
	}

	GameObject *go = m_goList.back();
	go->active = true;
	return go;
}

void SceneAsteroid::Update(double dt)
{
	//SceneBase::Update(dt);
	
}

int CGender; // Customer's Gender
const int FEMALE = 0; // State = Female
const int MALE = 1; // State = Male
int Females; // No. of females customers
int Males; // No. of male customers
float probability; 
int RandomIndex;
const float CustomerSpeed = 0.0275f;
const float SupplierSpeed = 0.0275f;
const float CustomerRadius = 0.2f;
const float SupplierRadius = 0.2f;
const float proximity = 0.4f; // ??
int wayPointIndex;
bool arrived;
MyVector CustomerPos, SupplierPos;
std::vector<MyVector>wayPoints, intrusionPoints;
std::vector<MyVector> stack;


void SceneAsteroid::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_CASHIER:
	{
		// Render Cashier
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CASHIER], false);
		modelStack.PopMatrix();
		break;
	}
	case GameObject::GO_CUSTOMER:
	{
		switch (CGender)
		{
		case FEMALE:
			// RenderMesh = GEO_FEMALE
			break;

		case MALE:
			// RenderMesh = GEO_MALE
			break;
		}
		break;
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

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth*0.5f, m_worldHeight*0.5f, -5);
	modelStack.Scale(m_worldWidth, m_worldHeight, 1);
	RenderMesh(meshList[GEO_BG], false);
	modelStack.PopMatrix();

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

	RenderGO(m_cashier);

	//On screen text
	if (m_score == 0 || m_score < 100)
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
	}

	std::ostringstream ss6;
	ss6.precision(5);
	ss6 << "Lives: " << m_lives;
	RenderTextOnScreen(meshList[GEO_TEXT], ss6.str(), Color(0, 1, 1), 3, 0, 55);

	std::ostringstream ss5;
	ss5.precision(5);
	ss5 << "Score: " << m_score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss5.str(), Color(0, 1, 1), 3, 16, 55);

}


void SceneAsteroid::Exit()
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
