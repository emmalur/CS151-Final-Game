#pragma once

class Entity
{
private:
    char symbol;
    
protected:
    int x;
    int y;

public:
    Entity(char symbol, int x, int y) : symbol{symbol}, x{x}, y{y} {}

    int getX() const { return x; }
    int getY() const { return y; }
    char getSymbol() const { return symbol; }
};
