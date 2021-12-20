#pragma once

#include "Position.h"
#include "GameObject.h"
#include "PanelRenderer.h"
#include "PlayerScript.h"
#include "Animation.h"
#include "MapScript.h"
#include "Obstacle.h"
using namespace std;

class Scene : public GameObject
{
    bool    isCompleted;
    Input*  input;
    GameObject* player;
    GameObject* map;
    GameObject* currentObstacle;
public:

    Scene() 
        : GameObject{ nullptr, "root", "root", nullptr, {1, 1}, {0, 0}, Position::zeros }, isCompleted(false),
        input( Input::GetInstance() )
    {   
        map = new GameObject(this, "map", "panel", nullptr, { 70, 20 }, Position::zeros, Position::zeros);
        map->getOrAddComponent<MapScript>();
        //player
        player = new GameObject{ map,"Player","Player",nullptr,{5,3} ,{0,10} ,Position::zeros };
        player->addComponent< PlayerScript>();
        player->getOrAddComponent<Animation>()->addAnimation("   \xc5 *****  ***");
        player->getComponent<Animation>()->addAnimation("   \xB3\xc4*****  ***");
        player->getComponent<Animation>()->addAnimation("   \xc5 *****  ***");
        player->getComponent<Animation>()->addAnimation("  \xc4\xB3 *****  ***");

        //obstacle
        auto obShape = Obstacle::ChooseShape();
        currentObstacle = new GameObject(map, "currentOBstacle", "block", obShape.shape.c_str(), obShape.dim, Position{ 70,5 });
        currentObstacle->addComponent<Obstacle>();
    }

    void start() override { internalStart(); }

    void update() override;

    bool isSceneOver() const { return isCompleted; }

#if 0
    void update() override {
        if (confirm && confirm->isActive() == false) {
            remove(confirm);
            delete confirm;
            confirm = nullptr;
        }
        if (input->getKey(VK_ESCAPE)) {
            auto dim = getDimension();
            confirm = new ConfirmationPanel{ getPos() + Position{ dim.x / 2 - 25/2, dim.y / 2 - 5/2}, 25, 5, this,
                [&]() { map->setFreeze(false); }, // if clicked cancel
                [&]() { isCompleted = true; } // if clicked okay
            };
            map->setFreeze(true);
        }
    }
#endif
};

