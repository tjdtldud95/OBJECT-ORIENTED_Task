#pragma once
#include "GameObject.h"
#include "Behaviour.h"
class UiScript :public Behaviour
{
	string buffer; //game over\0

public:
	UiScript(GameObject* gameObject) : Behaviour(gameObject)
	{
		buffer.append("Game Over");
	}

	void update() override
	{
		renderer->setShape(buffer.c_str());
	}

};

