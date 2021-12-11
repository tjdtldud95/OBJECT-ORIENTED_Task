#include "Scene.h"

void Scene::update() {    

    if (input->getKey(VK_ESCAPE)) {
        isCompleted = true;
        return;
    }

    internalRemove();
    internalRender(Position::zeros, false);
    internalUpdate();
}
