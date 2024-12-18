#pragma once

#include "level.h"
#include "cell.h"
#include "entity.h"

 
#include "ftxui/dom/elements.hpp"
#include <utility>

class Map
{

public:
    std::vector<Level> floors;
    Map(int numberOfFloors);
};

//std::vector<std::string> addEntities(Level &level, Player &player);

ftxui::Elements cellsToElements(std::vector<std::vector<Cell>> &cells, std::vector<Entity> &entities);

std::pair<int, int> suitibleLocation(Level &level, int roomNum);