#pragma once

#include "room.h"
#include "cell.h"
#include "enemy.h"

#include <vector>
#include <string>
#include <utility>
#include <random>

class Level
{
private:
    int floorNumber;
    std::vector<Room> rooms;

public:
    std::vector<std::vector<Cell>> floor;
    std::vector<Enemy> enemies;
    Level();

    void changeFloorNumber(int floorNumber) { this->floorNumber = floorNumber; }
    const Room& getRoom(int roomNum) const { return rooms[roomNum]; }
    // add character to level
    // move character

    void addRoom(Room room, bool visted);
    bool isMoveable(int x, int y);
    std::pair<int, int> suitibleLocation(int roomNum);
    void moveEnemies();
    Enemy getEnemyAt(int x, int y);
};
