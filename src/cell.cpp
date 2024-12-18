#include "cell.h"

Cell wall()
{
    Cell newWall;
    newWall.symbol = '#';
    newWall.isWalkable = false;
    newWall.isExplored = false;
    newWall.bgColor = ftxui::Color::GrayDark;
    newWall.fgColor = ftxui::Color::GrayLight;

    return newWall;
}

Cell ground()
{
    Cell newGround;
    newGround.symbol = '.';
    newGround.isWalkable = true;
    newGround.isExplored = false;
    newGround.bgColor = ftxui::Color::Black;
    newGround.fgColor = ftxui::Color::Yellow;

    return newGround;
}