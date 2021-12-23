#pragma once

#include "Position.h"
#include "GameObject.h"
#include "PanelRenderer.h"
#include "PlayerScript.h"
#include "Animation.h"
#include "MapScript.h"
#include "UiScript.h"
#include "ObstacleManager.h"
using namespace std;

class Scene : public GameObject
{
    bool    isCompleted;
    Input*  input;
    GameObject* player;
    GameObject* map;
    GameObject* obstacles;
    GameObject* gameOverUI;

public:

    Scene() 
        : GameObject{ nullptr, "root", "root", nullptr, {1, 1}, {0, 0}, Position::zeros }, isCompleted(false),
        input( Input::GetInstance() )
    {   
        //map
        map = new GameObject(this, "map", "panel", nullptr, { 70, 20 }, Position::zeros, Position::zeros);
        map->getOrAddComponent<MapScript>();

        //player
        player = new GameObject{ map,"Player","Player",nullptr,{5,3} ,{0,10} ,Position::zeros };
        player->addComponent< PlayerScript>();
        player->getOrAddComponent<Animation>()->addAnimation("   \xc5 \xc4\xc4\xc2\xd9\xBF  \xc0\xc4\xd9");
        player->getComponent<Animation>()->addAnimation("   \xB3\xc4\xc4\xc4\xc2\xd9\xBF  \xc0\xc4\xd9");
        player->getComponent<Animation>()->addAnimation("   \xc5 \xc4\xc4\xc2\xd9\xBF  \xc0\xc4\xd9");
        player->getComponent<Animation>()->addAnimation("  \xc4\xB3 \xc4\xc4\xc2\xd9\xBF  \xc0\xc4\xd9");

        //obstacle
        obstacles = new GameObject(map, "obstalce", "Manager", nullptr, Position::zeros, Position::zeros, Position::zeros);
        obstacles->getOrAddComponent<ObstacleManager>();

        //UI
        gameOverUI = new GameObject(this, "GameOver", "panel", nullptr, { 30,10 }, { 20,5 }, Position::zeros);
        gameOverUI->getOrAddComponent<PanelRenderer>();
        auto uiText = new GameObject(gameOverUI, "text", "text", nullptr, { 10,1 }, { 10,5 }, Position::zeros);
        auto uiScript = uiText->getOrAddComponent<UiScript>();


        gameOverUI->setActive(false);
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

