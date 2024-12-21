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
    int points;
    int level;

public:
    Player(int x, int y);
    Player();

    int getFloor() const { return floor; }
    void moveFloor(int amount) { floor += amount; }
    int getMaxHealth() const { return maxHealth; }
    int getDefense() const {return defense; }
    int getCoins() const { return coins; }
    int getPoints() const { return points; }
    int getLevel() const { return level; }

    void playerSetup(Map &map);
    void combat(Entity &opponent, int order, std::function<void(std::string)> addMessage, std::function<void()> lose);
};

std::string makeStatus(Player &player);