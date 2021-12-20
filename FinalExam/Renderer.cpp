#include "Renderer.h"
#include "Transform.h"
#include "GameObject.h"

void Renderer::draw() { 
    if (!shape) return;  screen->draw(worldPos, shape, dim);
}

void Renderer::render(const Position& parentWorldPos, bool dirty) {
    if (enabled == false) return;
    if (dirty == true) worldPos = transform->local2World(parentWorldPos);
    draw();
}