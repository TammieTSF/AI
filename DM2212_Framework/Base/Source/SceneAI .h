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

public:
	SceneAI();
	~SceneAI();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

	bool showInfo;
	void Menu();

protected:

	//Physics
	std::vector<GameObject *> m_goList;

	float m_speed;
	float m_worldWidth;
	float m_worldHeight;

	GameObject *m_cashier;

	Vector3 m_force;

	int m_objectCount;
	int m_lives;
	int m_score;
	int objectcount;
	int Selection;
	int Colour;
	bool StartGame;
	bool GameOver;
	bool Win;

};

#endif