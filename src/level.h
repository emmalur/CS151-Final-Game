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

    void bredthSearch(Enemy &enemy);

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
    void addHallway(int room1, int room2);
    void addStair(int x, int y);
    int whichRoom(int x, int y);
    Moveable isMoveable(int x, int y);
    std::pair<int, int> suitibleLocation(int roomNum);
    void moveEnemies(std::function<void(std::string)> addMessage, std::function<void()> lose);
    Enemy& getEnemyAt(int x, int y);
};
