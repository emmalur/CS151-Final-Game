#include "entity.h"
#include "level.h"

void Entity::move(int x, int y, Level &level)
{
    if (level.isMoveable(x, y) == Moveable::Moveable)
    {
        this->x = x;
        this->y = y;
        currentRoom = level.whichRoom(x, y);
    }
}