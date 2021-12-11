#pragma once
#include "Behaviour.h"
#include "GameObject.h"
#include "Transform.h"


class PlayerScript : public Behaviour
{
	int		currentX;
	int		currentY;
	float   aa=0.f;
public:

	PlayerScript(GameObject* gameObject) : Behaviour(gameObject)
		, currentX(0),currentY(0)
	{
		auto pos = transform->getPos();
		currentX = pos.x;
		currentY = pos.y;
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
		if (input->getKey(VK_DOWN))
		{
			currentY++;
			transform->setPos(currentX, currentY );
		}

		if (input->getKey(VK_UP))
		{
			currentY--;
			transform->setPos(currentX, currentY);
		}

		if (input->getKey(VK_RIGHT))
		{
			currentX++;
			transform->setPos(currentX, currentY);
		}
		if (input->getKey(VK_LEFT))
		{
			currentX--;
			transform->setPos(currentX, currentY);
		}
	}
};

