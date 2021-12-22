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
	MapScript* mapScript;
	GameObject* map;
	int currentX;
	int currentY;
	char* shapeBuffer;

public:
	Obstacle(GameObject* gameObject) :Behaviour(gameObject)
		, map(gameObject->getParent()),  shapeBuffer(nullptr), currentX(0), currentY(0)
	{
		mapScript = map->getComponent<MapScript>();
		auto pos = transform->getPos();
		auto dim = renderer->getDimension();
		currentX = (int)pos.x;
		currentY = (int)pos.y;
		shapeBuffer = new char[(size_t)dim.x * dim.y];
		transform->setPos((int)currentX, (int)currentY);
	}

	~Obstacle()
	{
		if (shapeBuffer) 
		{
			delete[] shapeBuffer;
		}
		currentY = 0;
		currentX = 0;
		map = nullptr;
		mapScript = nullptr;
	}
	static ObstacleShape ChooseShape() {
		static vector<ObstacleShape> candidates{
			{ "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB",	{2, 8}	}
		};
		return candidates[0];
	}

	void move()
	{
		auto shape = renderer->getShape();
		auto pos = transform->getPos();
		auto dim = renderer->getDimension();
		auto width = dim.x;
		auto height = dim.y;

		int nextX = currentX - 1;
		if (mapScript->isValidRange({ nextX,currentY }, dim))
		{
			currentX = nextX;
			transform->setPos(currentX, currentY);
			mapScript->place(shape, pos, width, height);
		}
		else
		{
			currentX = 69;
			currentY = rand() % 10 + 2;			
			transform->setPos(currentX, currentY);
			mapScript->place(shape, pos, width, height);
		}
	}

};

