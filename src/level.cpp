#include "level.h"
#include "player.h"

#include <iostream>
#include <utility>
#include <random>
#include <cmath>

struct PairHasher
{
    size_t operator()(const std::pair<int, int>& x) const
    {
        return x.first ^ x.second;
    }
};

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

    Player newPlayer;
    player = &newPlayer;

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

void Level::addHallway(int room1, int room2)
{
    // Find center of both rooms
    int center1X = rooms[room1].x + (rooms[room1].width / 2);
    int center1Y = rooms[room1].y + (rooms[room1].height / 2);

    int center2X = rooms[room2].x + (rooms[room2].width / 2);
    int center2Y = rooms[room2].y + (rooms[room2].height / 2);

    // When drawing hallway down
    if (center1Y < center2Y)
    {
        for (int i = center1Y; i <= center2Y; i++)
        {
            if (floor[i][center1X - 1].symbol != '.')
                floor[i][center1X - 1] = wall();
            if (floor[i][center1X + 1].symbol != '.')
                floor[i][center1X + 1] = wall();

            floor[i][center1X] = ground();
        }
    }
    // drawing hallway up
    else if (center1Y > center2Y)
    {
        for (int i = center2Y; i <= center1Y; i++)
        {
            if (floor[i][center1X - 1].symbol != '.')
                floor[i][center1X - 1] = wall();
            if (floor[i][center1X + 1].symbol != '.')
                floor[i][center1X + 1] = wall();

            floor[i][center1X] = ground();
        }
    }
    
    // When get to room2 y, go right with hallway line
    if (center1X < center2X)
    {
        for (int i = center1X; i < center2X; i++)
        {
            if (floor[center2Y - 1][i].symbol != '.')
                floor[center2Y - 1][i] = wall();
            if (floor[center2Y + 1][i].symbol != '.')
                floor[center2Y + 1][i] = wall();

            floor[center2Y][i] = ground();
        }
    }
    // go left with hallway
    else if(center1X > center2X)
    {
        for (int i = center1X; i > center2X; i--)
        {
            if (floor[center2Y - 1][i].symbol != '.')
                floor[center2Y - 1][i] = wall();
            if (floor[center2Y + 1][i].symbol != '.')
                floor[center2Y + 1][i] = wall();

            floor[center2Y][i] = ground();
        }
    }
}

void Level::addStair(int x, int y)
{
    Cell stair = ground();
    stair.symbol = '>';
    floor[y][x] = stair;
}

Moveable Level::isMoveable(int x, int y)
{
    // bounds checking
    if (x < 0 || x >= 120)
        return Moveable::No;
    if (y < 0 || y >= 24)
        return Moveable::No;
    
    if (x == player->getX() && y == player->getY())
        return Moveable::Player;

    for (Enemy &enemy : enemies)
    {
        // can't move if there's an enemy there
        if (x == enemy.getX() && y == enemy.getY())
        {
            return Moveable::Enemy;
        }
    }

    if (floor[y][x].isWalkable)
        return Moveable::Moveable;

    return Moveable::No;
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

        if (isMoveable(randPoint.first, randPoint.second) == Moveable::Moveable)
            return randPoint;
    }
}

int distance(int x, int y, Player *player)
{
    return abs(x - player->getX()) + abs(y - player->getY());
}

void moveTowards(Enemy &enemy, Player *player, Level &level, std::function<void(std::string)> addMessage, std::function<void()> lose)
{
    int x = enemy.getX();
    int y = enemy.getY();

    int currentDistance = distance(x, y, player);
    // shortest distance is not moving
    std::pair<int, int> shortestDistance = std::make_pair(currentDistance, 0);

    if (shortestDistance.first > distance(x, y - 1, player))
    {
        shortestDistance.first = distance(x, y - 1, player);
        shortestDistance.second = 1;
    }
    else if (shortestDistance.first > distance(x, y + 1, player))
    {
        shortestDistance.first = distance(x, y + 1, player);
        shortestDistance.second = 2;
    }
    else if (shortestDistance.first > distance(x + 1, y, player))
    {
        shortestDistance.first = distance(x + 1, y, player);
        shortestDistance.second = 3;
    }
    else if (shortestDistance.first > distance(x - 1, y, player))
    {
        shortestDistance.first = distance(x - 1, y, player);
        shortestDistance.second = 4;
    }

    switch (shortestDistance.second)
    {
        case 0:
            break;
        case 1:
            if (level.isMoveable(x, y - 1) == Moveable::Player)
            {
                player->combat(enemy, 1, addMessage, lose);
            }
            else
            {
                enemy.move(x, y - 1, level);
            }
            break;
        case 2:
            if (level.isMoveable(x, y + 1) == Moveable::Player)
            {
                player->combat(enemy, 1, addMessage, lose);
            }
            else
            {
                enemy.move(x, y + 1, level);
            }
            break;
        case 3:
            if (level.isMoveable(x + 1, y) == Moveable::Player)
            {
                player->combat(enemy, 1, addMessage, lose);
            }
            else
            {
                enemy.move(x + 1, y, level);
            }
            break;
        case 4:
            if (level.isMoveable(x - 1, y) == Moveable::Player)
            {
                player->combat(enemy, 1, addMessage, lose);
            }
            else
            {
                enemy.move(x - 1, y, level);
            }
            break;
    }
}

void moveRandomly(Enemy &enemy, Player *player, Level &level, std::function<void(std::string)> addMessage, std::function<void()> lose)
{
    // Random Number Generator
    std::random_device dev;
    std::mt19937 gen(dev());

    std::uniform_int_distribution movement {0, 4};
    switch(movement(gen))
    {
        case 0:
            // Do Nothing
            break;
        case 1:
            // UP
            if (level.isMoveable(enemy.getX(), enemy.getY() - 1) == Moveable::Moveable)
                enemy.move(enemy.getX(), enemy.getY() - 1, level);
            else if (level.isMoveable(enemy.getX(), enemy.getY() - 1) == Moveable::Player)
                player->combat(enemy, 1, addMessage, lose);
            break;
        case 2:
            // Down
            if (level.isMoveable(enemy.getX(), enemy.getY() + 1) == Moveable::Moveable)
                enemy.move(enemy.getX(), enemy.getY() + 1, level);
            else if (level.isMoveable(enemy.getX(), enemy.getY() + 1) == Moveable::Player)
                player->combat(enemy, 1, addMessage, lose);
            break;
        case 3:
            // Left
            if (level.isMoveable(enemy.getX() - 1, enemy.getY()) == Moveable::Moveable)
                enemy.move(enemy.getX() - 1, enemy.getY(), level);
            else if (level.isMoveable(enemy.getX() - 1, enemy.getY()) == Moveable::Player)
                player->combat(enemy, 1, addMessage, lose);
            break;
        case 4:
            // Right
            if (level.isMoveable(enemy.getX() + 1, enemy.getY()) == Moveable::Moveable)
                enemy.move(enemy.getX() + 1, enemy.getY(), level);
            else if (level.isMoveable(enemy.getX() + 1, enemy.getY()) == Moveable::Player)
                player->combat(enemy, 1, addMessage, lose);
            break;
    }
}

void Level::moveEnemies(std::function<void(std::string)> addMessage, std::function<void()> lose)
{
    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
        Enemy &enemy = enemies[i];
        // Remove enemies who have died from the game
        if (enemy.getHealth() <= 0)
        {
            // Swap current enemy to front
            Enemy temp = enemies[enemies.size() - 1];
            enemies[enemies.size() - 1] = enemy;
            enemies[i] = temp;

            // Remove enemy
            enemies.pop_back();
        }

        switch (enemy.getType())
        {
            case EnemyType::Spider:
                // Move the spider in a random direction, one of which being nothing
                moveRandomly(enemy, player, *this, addMessage, lose);
                break;
            case EnemyType::Zombie:
                if (player->getRoom() == enemy.getRoom())
                {
                    moveTowards(enemy, player, *this, addMessage, lose);
                }
                else
                {
                    moveRandomly(enemy, player, *this, addMessage, lose);
                }
                break;
            case EnemyType::Giant:
                if (player->getRoom() == enemy.getRoom())
                {
                    moveTowards(enemy, player, *this, addMessage, lose);
                }
                else
                {
                    moveRandomly(enemy, player, *this, addMessage, lose);
                }
                break;
            case EnemyType::Dragon:
                if (player->getRoom() == enemy.getRoom())
                {
                    moveTowards(enemy, player, *this, addMessage, lose);
                }
                else
                {
                    moveRandomly(enemy, player, *this, addMessage, lose);
                }
                break;
            default:
                moveTowards(enemy, player, *this, addMessage, lose);
                break;
        }
    }
}

Enemy& Level::getEnemyAt(int x, int y)
{
    for (Enemy &enemy : enemies)
    {
        if (enemy.getX() == x && enemy.getY() == y)
            return enemy;
    }

    // couldn't find enemy there; not sure what to return
    // return null;
}

int Level::whichRoom(int x, int y)
{
    for (int roomNum = 0; roomNum < static_cast<int>(rooms.size()); roomNum++)
    {
        if (x > rooms[roomNum].x && x < rooms[roomNum].x + rooms[roomNum].width)
        {
            if (y > rooms[roomNum].y && y < rooms[roomNum].y + rooms[roomNum].height)
                return roomNum;
        }
    }

    return -1;
}