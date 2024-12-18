#pragma once

#include "entity.h"
#include "level.h"
#include "map.h"

class Player : public Entity
{
private:
    int floor;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int coins;

public:
    Player(int x, int y);

    int getFloor() const { return floor; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getAttack() const { return attack; }
    int getDefense() const {return defense; }
    int getCoins() const { return coins; }

    void moveVertical(int direction, Level &level);
    void moveHorizontal(int direction, Level &level);
};

Player playerSetup(Map &map);
std::string makeStatus(Player &player);