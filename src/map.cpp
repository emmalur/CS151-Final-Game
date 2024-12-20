#include "map.h"

#include <random>

int const MAX_ROOM_HEIGHT = 12;
int const MAX_ROOM_WIDTH = 40;

// Random Number Generator
std::random_device dev;
std::mt19937 gen(dev());

Map::Map(int numberOfFloors, Player &player)
{
    std::vector<Level> newFloors(numberOfFloors);

    // give each level the correct floor number and add player
    for (int i = 0; i < static_cast<int>(newFloors.size()); i++)
    {
        newFloors[i].changeFloorNumber(i);
        newFloors[i].addPlayer(player);
    }

    // add correct amount of rooms to each level
    for (int i = 0; i < numberOfFloors; i++)
    {
        for (int roomNum = 0; roomNum < 6; roomNum++)
        {
            Room newRoom;

            // set top left corner of newRoom depending on which part of grid
            switch (roomNum)
            {
                case 0:
                    newRoom.x = 0;
                    newRoom.y = 0;
                    break;
                case 1:
                    newRoom.x = 40;
                    newRoom.y = 0;
                    break;
                case 2:
                    newRoom.x = 80;
                    newRoom.y = 0;
                    break;
                case 3:
                    newRoom.x = 80;
                    newRoom.y = 12;
                    break;
                case 4:
                    newRoom.x = 40;
                    newRoom.y = 12;
                    break;
                case 5:
                    newRoom.x = 0;
                    newRoom.y = 12;
            }

            std::uniform_int_distribution randomHeight {5, 10};
            newRoom.height = randomHeight(gen);

            std::uniform_int_distribution randomWidth {5, 25};
            newRoom.width = randomWidth(gen);

            // x y offset
            std::uniform_int_distribution randomXOffset {0, MAX_ROOM_WIDTH - newRoom.width - 1};
            newRoom.x += randomXOffset(gen);

            std::uniform_int_distribution randomYOffset {0, MAX_ROOM_HEIGHT - newRoom.height - 1};
            newRoom.y += randomYOffset(gen);

            if (roomNum == 0 && i == 0)
            {
                newFloors[0].addRoom(newRoom, true);
            }
            else
            {
                // set to true if you want to see all rooms
                newFloors[i].addRoom(newRoom, true);
            }

            if (roomNum != 0)
            {
                newFloors[i].addHallway(roomNum - 1, roomNum);
            }
        }

        // Add stairs to each level
        std::uniform_int_distribution stairRoom {0, 5};
        std::pair<int, int> stairLoc = newFloors[i].suitibleLocation(stairRoom(gen));
        newFloors[i].addStair(stairLoc.first, stairLoc.second);

    }

    std::uniform_int_distribution monsterDist {0, 7};

    // Add enemies to each level
    for (int i = 0; i < numberOfFloors; i++)
    {
        if (i == 0 || (i + 1) % 5 != 0 )
        {
            // Only spiders and zombies
            if ((i + 1) < 5)
            {
                for (int roomNum = 0; roomNum < 6; roomNum++)
                {
                    int monsterChance = monsterDist(gen);
                    // 50% chance of a monster in each room, 50% chance it's either spider or zombie
                    switch (monsterChance)
                    {
                        case 0:
                        case 2:
                        {
                            std::pair<int, int> startLoc = newFloors[i].suitibleLocation(roomNum);
                            Enemy newEnemy(EnemyType::Spider, startLoc.first, startLoc.second);
                            newFloors[i].enemies.push_back(newEnemy);
                        }
                            break;
                        case 4:
                        case 6:
                        {
                            std::pair<int, int> startLoc = newFloors[i].suitibleLocation(roomNum);
                            Enemy newEnemy(EnemyType::Zombie, startLoc.first, startLoc.second);
                            newFloors[i].enemies.push_back(newEnemy);
                        }
                            break;
                        default:
                            break;
                    }
                }
            }
            // Giants and zombies
            else if (5 <= i && i < 8)
            {
                for (int roomNum = 0; roomNum < 6; roomNum++)
                {
                    int monsterChance = monsterDist(gen);
                    // 100% chance of an Enemy, 50% chance it's either giant or zombie
                    switch (monsterChance)
                    {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        {
                            std::pair<int, int> startLoc = newFloors[i].suitibleLocation(roomNum);
                            Enemy newEnemy(EnemyType::Giant, startLoc.first, startLoc.second);
                            newFloors[i].enemies.push_back(newEnemy);
                        }
                            break;
                        case 4:
                        case 5:
                        case 6:
                        case 7:
                        {
                            std::pair<int, int> startLoc = newFloors[i].suitibleLocation(roomNum);
                            Enemy newEnemy(EnemyType::Zombie, startLoc.first, startLoc.second);
                            newFloors[i].enemies.push_back(newEnemy);
                        }
                            break;
                        default:
                            break;
                    }
                }
            }
            // Giants and dragons
            else if (i >= 8)
            {
                for (int roomNum = 0; roomNum < 6; roomNum++)
                {
                    int monsterChance = monsterDist(gen);
                    // 100% chance of a monster in each room, 75% chance it's a giant and 25% dragon
                    switch (monsterChance)
                    {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        {
                            std::pair<int, int> startLoc = newFloors[i].suitibleLocation(roomNum);
                            Enemy newEnemy(EnemyType::Giant, startLoc.first, startLoc.second);
                            newFloors[i].enemies.push_back(newEnemy);
                        }
                            break;
                        case 6:
                        case 7:
                        {
                            std::pair<int, int> startLoc = newFloors[i].suitibleLocation(roomNum);
                            Enemy newEnemy(EnemyType::Dragon, startLoc.first, startLoc.second);
                            newFloors[i].enemies.push_back(newEnemy);
                        }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    floors = newFloors;
}

using namespace ftxui;
ftxui::Elements cellsToElements(std::vector<std::vector<Cell>> &cells, std::vector<Entity> &entities)
{
    Elements page;
    for (int y = 0; y < static_cast<int>(cells.size()); y++)
    {
        Elements row;
        for (int x = 0; x < static_cast<int>(cells[y].size()); x++)
        {
            // don't show places the player hasn't been yet
            if (cells[y][x].isExplored)
            {
                bool wasEntity = false;
                // An entity could be there
                if (cells[y][x].isWalkable)
                {
                    for (Entity entity : entities)
                    {
                        if (entity.getHealth() <= 0)
                            continue;

                        if (entity.getX() == x && entity.getY() == y)
                        {
                            std::string s{entity.getSymbol()};
                            row.push_back(
                                text(s) | bgcolor(ftxui::Color::Black) | color(ftxui::Color::White)
                            );
                            wasEntity = true;
                        }
                    }
                }
                if (!wasEntity)
                {
                    // convert character to string
                    std::string s{cells[y][x].symbol};
                    row.push_back(
                        text(s) | bgcolor(cells[y][x].bgColor) | color(cells[y][x].fgColor)
                    );
                }
            }
            else
            {
                row.push_back( text(" "));
            }
        }
        page.push_back(hbox(std::move(row)));
    }

    return page;
}