#pragma once
#include "Behaviour.h"
#include "GameObject.h"
class Animation //: public Renderer
	:public Behaviour
{
	vector<string> ani;
	int aniIndex;

public :
	Animation(GameObject* gameObject) :Behaviour(gameObject),
		aniIndex(0) {}

	void addAnimation(const string shape)
	{
		ani.push_back(shape);
	}


	void update() override
	{
		if (ani.size() == 0) return;

		renderer->setShape(ani[aniIndex].c_str());
		aniIndex = (aniIndex + 1) % ani.size();
	}
};

