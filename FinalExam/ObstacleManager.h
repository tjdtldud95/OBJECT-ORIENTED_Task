#pragma once
#include "Behaviour.h"
#include "GameObject.h"
#include "Obstacle.h"

class ObstacleManager :public Behaviour
{
	vector<GameObject*> obs;
	GameObject* map;
	MapScript* mapScript;
	float	playTime;

public :
	ObstacleManager(GameObject* gameObject) :Behaviour(gameObject)
		, map(gameObject->getParent()), playTime(0.f)
	{
		mapScript = map->getOrAddComponent<MapScript>();
	}

	~ObstacleManager()
	{
		mapScript = nullptr;
		map = nullptr;
		obs.clear();
	}

	void start() override
	{
		addObstacle(Position{ 65,0 });
		addObstacle(Position{ 43,0 });
		addObstacle(Position{ 25,0 });
		addObstacle(Position{ 10,0 });
	}

	void update() override
	{
		mapScript->clear();
		for_each(obs.begin(), obs.end(), [&](auto it) {obstaclePlace(it);});

		playTime += 0.103f;
		Borland::Gotoxy(0, 22);
		printf("playTime : %.3f\n", playTime);
	}

	void obstaclePlace(GameObject* obstacle)
	{
		obstacle->getComponent<Obstacle>()->move();
	}

	void addObstacle(const Position& pos)
	{
		auto obShape = Obstacle::ChooseShape();
		auto obstacle = new GameObject(map, "Obstacle", "Obstacle", obShape.shape.c_str(), obShape.dim, pos);
		obstacle->getOrAddComponent<Obstacle>();

		obs.push_back(obstacle);
	}

};

