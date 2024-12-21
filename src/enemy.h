#pragma once

#include "entity.h"

enum class EnemyType
{
    Zombie,
    Spider,
    Giant,
    Dragon,
    Custom,
};

class Enemy : public Entity
{
private:
    EnemyType type;
    int defense;

public:
    Enemy();
    Enemy(EnemyType type, int x, int y);

    EnemyType getType() const { return type; }
};