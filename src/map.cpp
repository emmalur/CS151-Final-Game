#include "map.h"

int const MAX_ROOM_HEIGHT = 12;
int const MAX_ROOM_WIDTH = 40;

// Random Number Generator
std::random_device dev;
std::mt19937 gen(dev());

Map::Map(int numberOfFloors)
{
    std::vector<Level> newFloors(numberOfFloors);

    // give each level the correct floor number
    for (int i = 0; i < static_cast<int>(newFloors.size()); i++)
    {
        newFloors[i].changeFloorNumber(i);
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
                    newRoom.x = 0;
                    newRoom.y = 12;
                    break;
                case 4:
                    newRoom.x = 40;
                    newRoom.y = 12;
                    break;
                case 5:
                    newRoom.x = 80;
                    newRoom.y = 12;
            }

            std::uniform_int_distribution randomHeight {5, 10};
            newRoom.height = randomHeight(gen);

            std::uniform_int_distribution randomWidth {5, 25};
            newRoom.width = randomWidth(gen);

            // x y offset
            std::uniform_int_distribution randomXOffset {0, MAX_ROOM_WIDTH - newRoom.width};
            newRoom.x += randomXOffset(gen);

            std::uniform_int_distribution randomYOffset {0, MAX_ROOM_HEIGHT - newRoom.height - 1};
            newRoom.y += randomYOffset(gen);

            newFloors[i].addRoom(newRoom, true);
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