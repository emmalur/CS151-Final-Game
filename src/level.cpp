#include "level.h"

#include <iostream>

const int GAME_WIDTH = 120;
const int GAME_HEIGHT = 24;

Level::Level()
{
    std::vector<Cell> row(GAME_WIDTH);
    std::vector<std::vector<Cell>> cells(GAME_HEIGHT, row);

    for (std::vector<Cell> v : cells)
    {
        for (Cell cell : v)
        {
            cell.symbol = ' ';
            cell.isExplored = false;
            cell.isWalkable = false;
            cell.bgColor = ftxui::Color::Black;
            cell.fgColor = ftxui::Color::White;
        }
    }

    // std::vector<Enemy> e{};
    enemies = {};

    floor = cells;
    floorNumber = 0;

    rooms = std::vector<Room> {};
}

void Level::addRoom(Room room, bool visted)
{
    rooms.push_back(room);

    // add it to floor
    for (int i = 0; i < room.height; i++)
    {
        for (int j = 0; j < room.width; j++)
        {
            if (i == 0 || i == room.height - 1)
            {
                floor[room.y + i][room.x + j] = wall();
                floor[room.y + i][room.x + j].isExplored = visted;
                continue;
            }

            if (j == 0 || j == room.width - 1)
            {
                floor[room.y + i][room.x + j] = wall();
                floor[room.y + i][room.x + j].isExplored = visted;
            }
            else
            {
                floor[room.y + i][room.x + j] = ground();
                floor[room.y + i][room.x + j].isExplored = visted;
            }
        }
    }
}

bool Level::isMoveable(int x, int y)
{
    // bounds checking
    if (x < 0 || x >= 120)
        return false;
    if (y < 0 || y >= 24)
        return false;

    for (Enemy enemy : enemies)
    {
        // can't move if there's an enemy there
        if (x == enemy.getX() && y == enemy.getY())
            return false;
    }

    return floor[y][x].isWalkable;
}

std::pair<int, int> Level::suitibleLocation(int roomNum)
{
    // Random Number Generator
    std::random_device dev;
    std::mt19937 gen(dev());

    std::uniform_int_distribution xDist {rooms[roomNum].x + 1, rooms[roomNum].x + rooms[roomNum].width - 1};
    std::uniform_int_distribution yDist {rooms[roomNum].y + 1, rooms[roomNum].y + rooms[roomNum].height - 1};

    std::pair<int, int> randPoint;
    while (true)
    {
        randPoint.first = xDist(gen);
        randPoint.second = yDist(gen);

        if (isMoveable(randPoint.first, randPoint.second))
            return randPoint;
    }
}

void Level::moveEnemies()
{
    // This isn't working; the enemy types are random and I'm not sure why

    // Random Number Generator
    std::random_device dev;
    std::mt19937 gen(dev());

    for (Enemy &enemy : enemies)
    {
        switch (enemy.getType())
        {
            case EnemyType::Spider:
                // Move the spider in a random direction, one of which being nothing
                {
                    std::uniform_int_distribution movement {0, 4};
                    switch(movement(gen))
                    {
                        case 0:
                            // Do Nothing
                            break;
                        case 1:
                            // UP
                            if (isMoveable(enemy.getX(), enemy.getY() - 1))
                                enemy.move(enemy.getX(), enemy.getY() - 1, *this);
                            break;
                        case 2:
                            // Down
                            if (isMoveable(enemy.getX(), enemy.getY() + 1))
                                enemy.move(enemy.getX(), enemy.getY() - 1, *this);
                            break;
                        case 3:
                            // Left
                            if (isMoveable(enemy.getX() - 1, enemy.getY()))
                                enemy.move(enemy.getX() - 1, enemy.getY(), *this);
                            break;
                        case 4:
                            // Right
                            if (isMoveable(enemy.getX() + 1, enemy.getY()))
                                enemy.move(enemy.getX() + 1, enemy.getY(), *this);
                            break;
                    }
                }
                break;
            default:
                {
                    std::uniform_int_distribution movement {0, 4};
                    switch(movement(gen))
                    {
                        case 0:
                            // Do Nothing
                            break;
                        case 1:
                            // UP
                            if (isMoveable(enemy.getX(), enemy.getY() - 1))
                                enemy.move(enemy.getX(), enemy.getY() - 1, *this);
                            break;
                        case 2:
                            // Down
                            if (isMoveable(enemy.getX(), enemy.getY() + 1))
                                enemy.move(enemy.getX(), enemy.getY() - 1, *this);
                            break;
                        case 3:
                            // Left
                            if (isMoveable(enemy.getX() - 1, enemy.getY()))
                                enemy.move(enemy.getX() - 1, enemy.getY(), *this);
                            break;
                        case 4:
                            // Right
                            if (isMoveable(enemy.getX() + 1, enemy.getY()))
                                enemy.move(enemy.getX() + 1, enemy.getY(), *this);
                            break;
                    }
                }
                break;
        }
    }
}

Enemy Level::getEnemyAt(int x, int y)
{
    for (Enemy enemy : enemies)
    {
        if (enemy.getX() == x && enemy.getY() == y)
            return enemy;
    }

    // couldn't find
    return Enemy();
}