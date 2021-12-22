#pragma once
#include "Behaviour.h"
#include "GameObject.h"
#include "Obstacle.h"

class ObstacleManager :public Behaviour
{
	vector<GameObject*> obs;
	GameObject* map;
	MapScript* mapScript;

public :
	ObstacleManager(GameObject* gameObject) :Behaviour(gameObject)
		, map(gameObject->getParent())
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
		
		for (int i = 0;i < obs.size();i++)
			obstaclePlace(obs[i]);
	}

	void obstaclePlace(GameObject* obstacle)
	{
		obstacle->getComponent<Obstacle>()->move();
	}

	void addObstacle(const Position& pos)
	{
		auto obShape = Obstacle::ChooseShape();
		auto obstacle = new GameObject(map, "Obstacle", "block", obShape.shape.c_str(), obShape.dim, pos);
		obstacle->getOrAddComponent<Obstacle>();

		obs.push_back(obstacle);
	}


};

