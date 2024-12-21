#pragma once

#include "entity.h"

#include <string>

class Map;

class Player : public Entity
{
private:
    int floor;
    int maxHealth;
    int defense;
    int coins;

public:
    Player(int x, int y);
    Player();

    int getFloor() const { return floor; }
    int getMaxHealth() const { return maxHealth; }
    int getDefense() const {return defense; }
    int getCoins() const { return coins; }

    void playerSetup(Map &map);
};

std::string makeStatus(Player &player);