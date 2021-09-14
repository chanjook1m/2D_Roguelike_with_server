#pragma once
#ifndef GAMEPLAY_H_INCLUDED
#define GAMEPLAY_H_INCLUDED


#include <memory>
#include <array>

#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>
#include <SFML\Network.hpp>

#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "random.h"
#include "ingameText.h"
#include "item.h"
#include "wall.h"
#include "animatedGIF.h"
#include "client_interface.hpp"
#define RESOURCE_DIR (string)"C:\\Users\\1z3r0\\Desktop\\game\\2D_Roguelike\\Resources\\"

#define STRESS_TEST false

using namespace std;

#include "Game.hpp"
#include "State.hpp"

class GamePlay : public Engine::State
{
private:
    std::shared_ptr<Context> m_context;

public:
    GamePlay(std::shared_ptr<Context>& context);
    ~GamePlay();

    void Init() override;
    void ProcessInput() override;
    void Update(sf::Time deltaTime) override;
    void Draw() override;
};
#endif