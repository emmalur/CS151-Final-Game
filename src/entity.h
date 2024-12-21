#pragma once

#include <functional>
#include <string>

class Level;

class Entity
{  
protected:
    char symbol;
    int x;
    int y;
    int currentRoom;
    int health;
    int attack;
    std::string name;

public:
    Entity(char symbol, int x, int y, int health, int attack, const std::string& name) : 
        symbol{symbol}, x{x}, y{y},currentRoom{0}, health{health}, attack{attack}, name{name}
    {}

    int getX() const { return x; }
    int getY() const { return y; }
    char getSymbol() const { return symbol; }
    int getHealth() const { return health; }
    int getAttack() const { return attack; }
    const std::string& getName() const { return name; }
    int getRoom() const { return currentRoom; }

    void changeRoom(int roomNum) { currentRoom = roomNum; }
    void changeHealth(int amount) { health += amount; }

    void move(int x, int y, Level &level);
};
