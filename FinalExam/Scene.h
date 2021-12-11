#pragma once

#include "Position.h"
#include "GameObject.h"
#include "PanelRenderer.h"
#include "PlayerScript.h"
#include "Animation.h"
using namespace std;

class Scene : public GameObject
{
    bool    isCompleted;
    Input*  input;
    GameObject* player;
public:

    Scene() 
        : GameObject{ nullptr, "root", "root", nullptr, {1, 1}, {0, 0}, Position::zeros }, isCompleted(false),
        input( Input::GetInstance() )
    {
        player = new GameObject{ this,"Player","Player",nullptr,{5,3} ,Position::zeros ,Position::zeros };
        player->addComponent< PlayerScript>();
        player->getOrAddComponent<Animation>()->addAnimation("  -*-*****  ***", { 5,5 });
        player->getComponent<Animation>()->addAnimation("   * *****  ***", { 5,5 });
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

