#pragma once
#include "GameObject.h"
#include "Behaviour.h"
#include "MapScript.h"

struct ObstacleShape
{
	string shape;
	Dimension dim;
};

class Obstacle :
	public Behaviour
{
	MapScript*  mapScript;
	GameObject* map;
	int currentX;
	int currentY;
	char* shapeBuffer;

public:
	Obstacle(GameObject* gameObject) :Behaviour(gameObject)
		, map(gameObject->getParent()),  shapeBuffer(nullptr), currentX(0), currentY(0)
	{
		auto pos = transform->getPos();
		auto dim = renderer->getDimension();
		currentX = (int)pos.x;
		currentY = (int)pos.y;
		mapScript = map->getComponent<MapScript>();
		shapeBuffer = new char[(size_t)dim.x * dim.y];
		srand(time(nullptr));
	}


	static ObstacleShape ChooseShape() {
		static vector<ObstacleShape> candidates{
			{ "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB",	{2, 9}	}
		};
		return candidates[rand() % candidates.size()];
	}
	void start() override {
		//currentX = 60.0f; currentY = 5.0f;
		gameObject->setName("currentBlock");
		transform->setPos((int)currentX, (int)currentY);
	}
	void update() override
	{
		auto shape = renderer->getShape();
		auto pos = transform->getPos();
		auto dim = renderer->getDimension();
		auto width = dim.x;
		auto height = dim.y;

		int nextX = currentX - 1;
		if (mapScript->isValidRange({ nextX,currentY }, dim) )
		{
			currentX = nextX;
			transform->setPos(currentX, currentY);
			mapScript->clear();
			mapScript->place(shape, pos, width, height);
		}	

		else
		{
			mapScript->clear();
			currentX = 69;
			currentY = rand() % 10 + 2;
			transform->setPos(currentX, currentY);
		}
	}

};

