#pragma once
#include "Renderer.h"
#include "GameObject.h"
class Animation : public Renderer
{
	Transform* transform;
	vector<string> ani;
	int aniIndex;
	int indexMax;

public :
	Animation(GameObject* gameObject)
		: Renderer(gameObject, nullptr, gameObject->getRenderer()->getDimension() + Position::ones), aniIndex(0), indexMax(0)
		, transform(gameObject->getTransform()) {}

	void addAnimation(const string shape,Dimension dim)
	{
		ani.push_back(shape);
		
		indexMax++;
	}

	void draw() override
	{
		if (indexMax == 0) return;

		screen->draw(transform->getPos(), ani[aniIndex].c_str(), { 5,3 });
		aniIndex = (aniIndex + 1) % indexMax;
	}
};

