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

			return;
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
			if (currentX <= 0) return;
			currentX--;
			transform->setPos(currentX, currentY);
		}
	}
};

