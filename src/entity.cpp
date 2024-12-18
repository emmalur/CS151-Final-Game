#include "entity.h"
#include "level.h"

void Entity::move(int x, int y, Level &level)
{
    if (level.isMoveable(x, y))
    {
        this->x = x;
        this->y = y;
    }
}