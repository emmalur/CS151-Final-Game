#include "enemy.h"

Enemy::Enemy() : Entity('E', 7, 7)
{
    health = 10;
    attack = 10;
    defense = 10;
    type = EnemyType::Custom;
}

Enemy::Enemy(EnemyType type, int x, int y) : Entity('E', x, y), type{type}
{
    switch (type)
    {
        case EnemyType::Zombie:
            symbol = 'Z';
            health = 15;
            attack = 4;
            defense = 4;

            break;
        case EnemyType::Spider:
            symbol = 'X';
            health = 10;
            attack = 2;
            defense = 1;
            break;
        case EnemyType::Giant:
            symbol = 'G';
            health = 25;
            attack = 10;
            defense = 5;
            break;
        case EnemyType::Dragon:
            symbol = 'D';
            health = 25;
            attack = 15;
            defense = 12;
            break;
        default:
            break;
    }
}