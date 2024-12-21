#include "enemy.h"

Enemy::Enemy() : Entity('E', 0, 0, 0, 0, "enemy")
{
    health = 10;
    attack = 10;
    defense = 10;
    type = EnemyType::Custom;
}

Enemy::Enemy(EnemyType type, int x, int y) : Entity('E', x, y, 0, 0, "enemy"), type{type}
{
    switch (type)
    {
        case EnemyType::Zombie:
            symbol = 'Z';
            health = 15;
            attack = 4;
            defense = 4;
            name = "Zombie";
            break;
        case EnemyType::Spider:
            symbol = 'X';
            health = 10;
            attack = 2;
            defense = 1;
            name = "Spider";
            break;
        case EnemyType::Giant:
            symbol = 'G';
            health = 25;
            attack = 10;
            defense = 5;
            name = "Giant";
            break;
        case EnemyType::Dragon:
            symbol = 'D';
            health = 25;
            attack = 15;
            defense = 12;
            name = "Dragon";
            break;
        default:
            break;
    }
}