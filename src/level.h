#pragma once

#include "room.h"
#include "cell.h"
#include "enemy.h"
#include "player.h"

#include <vector>
#include <string>
#include <functional>

enum class Moveable
{
    No,
    Enemy,
    Player,
    Moveable,
};

class Level
{
private:
    int floorNumber;
    std::vector<Room> rooms;
    Player *player;

public:
    std::vector<std::vector<Cell>> floor;
    std::vector<Enemy> enemies;

    Level();

    void changeFloorNumber(int floorNumber) { this->floorNumber = floorNumber; }
    const Room& getRoom(int roomNum) const { return rooms[roomNum]; }
    void addPlayer(Player &player) { this->player = &player; }
    // add character to level
    // move character

    void addRoom(Room room, bool visted);
    Moveable isMoveable(int x, int y);
    std::pair<int, int> suitibleLocation(int roomNum);
    void moveEnemies(std::function<void(std::string)> addMessage);
    Enemy& getEnemyAt(int x, int y);
};
