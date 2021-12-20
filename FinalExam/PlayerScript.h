#pragma once
#include "Behaviour.h"
#include "GameObject.h"
#include "Transform.h"
#include "MapScript.h"

class PlayerScript : public Behaviour
{
	MapScript* mapScript;
	GameObject* map;
	int		currentX;
	int		currentY;
public:

	PlayerScript(GameObject* gameObject) : Behaviour(gameObject)
		,map(gameObject->getParent()), currentX(0),currentY(0)
	{
		auto pos = transform->getPos();
		currentX = pos.x;
		currentY = pos.y;

		mapScript = map->getComponent<MapScript>();
	}

	~PlayerScript()
	{ }

	void start() override
	{
		gameObject->setName("Player");
		transform->setPos(currentX, currentY);
	}

	void update() override
	{
		auto dim = renderer->getDimension();
		auto shape = renderer->getShape();
		auto pos = transform->getPos();
		auto width = dim.x;
		auto height = dim.y;

		if (mapScript->isCruched(shape, pos, width, height))
		{
			Borland::Gotoxy(5, 35);
			printf("Cruch");
			Borland::Gotoxy(0, 0);
		}
		else
		{
			Borland::Gotoxy(5, 35);
			printf("not  ");
			Borland::Gotoxy(0, 0);
		}

		if (input->getKey(VK_DOWN))
		{
			int nextY = currentY + 1;
			if (mapScript->isValidRange({ currentX,nextY }, dim))
			{
				currentY = nextY;
				transform->setPos(currentX, currentY);
			}
		}

		if (input->getKey(VK_UP))
		{
			int nextY = currentY -1;
			if (mapScript->isValidRange({ currentX,nextY }, dim))
			{
				currentY = nextY;
				transform->setPos(currentX, currentY);
			}
		}

		if (input->getKey(VK_RIGHT))
		{
			int nextX = currentX+1;
			if (mapScript->isValidRange({ nextX,currentY }, dim))
			{
				currentX = nextX;
				transform->setPos(currentX, currentY);
			}

		}
		if (input->getKey(VK_LEFT))
		{
			int nextX = currentX - 1;
			if (mapScript->isValidRange({ nextX,currentY }, dim))
			{
				currentX = nextX;
				transform->setPos(currentX, currentY);
			}
		}
	}
};

