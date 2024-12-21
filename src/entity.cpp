#include "entity.h"
#include "level.h"

void Entity::move(int x, int y, Level &level)
{
    if (level.isMoveable(x, y) == Moveable::Moveable)
    {
        this->x = x;
        this->y = y;
    }
}

void Entity::combat(Entity &attacker, std::function<void(std::string)> addMessage)
{
    int attackDamage = attacker.getAttack();
    if ((health - attackDamage) > 0)
    {
        health -= attackDamage;
        addMessage(this->name + " got attacked for " + std::to_string(attackDamage) + " by " + attacker.getName());
    }
    else
    {
        health -= attackDamage;
        addMessage(attacker.getName() + " killed " + this->name + "!");
    }
}