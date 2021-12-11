#pragma once
#include "Input.h"
#include "Component.h"

class Renderer;

class Behaviour :
    public Component
{
protected:
    Input*      input;
    Renderer*   renderer;
public:
    Behaviour(GameObject* gameObject);

    virtual ~Behaviour() {}
};

