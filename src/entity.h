#pragma once

class Level;

class Entity
{  
protected:
    char symbol;
    int x;
    int y;

public:
    Entity(char symbol, int x, int y) : symbol{symbol}, x{x}, y{y} {}

    int getX() const { return x; }
    int getY() const { return y; }
    char getSymbol() const { return symbol; }

    void move(int x, int y, Level &level);
};
