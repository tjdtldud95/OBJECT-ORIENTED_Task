#include "Scene.h"

void Scene::update() {    

    if (input->getKey(VK_ESCAPE)) {
        isCompleted = true;
        return;
    }

    if (!player->isAlive())
    {
        return;
    }

    internalRemove();
    internalRender(Position::zeros, false);
    internalUpdate();
}
