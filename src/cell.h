#pragma once

#include <ftxui/screen/terminal.hpp>
#include "ftxui/screen/color.hpp"

struct Cell
{
    char symbol;
    // has the player been here
    bool isExplored;
    bool isWalkable;

    ftxui::Color bgColor;
    ftxui::Color fgColor; 
};

Cell wall();
Cell ground();