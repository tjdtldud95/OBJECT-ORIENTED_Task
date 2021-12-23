#include "Scene.h"

void Scene::update() {    

    if (input->getKey(VK_ESCAPE)) {
        isCompleted = true;
        return;
    }

    if (!player->isAlive())
    {
       map->setActive(false);
       obstacles->setActive(false);
       player->setActive(false);
       gameOverUI->setActive(true);
    }

    internalRemove();
    internalRender(Position::zeros, false);
    internalUpdate();
}
