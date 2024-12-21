#include "player.h"
#include "map.h"

Player::Player() : Entity('@', 0, 0, 0, 0, "Player")
{
    floor = 0;
    health = 50;
    maxHealth = 50;
    attack = 10;
    defense = 5;
    coins = 0;
    points = 0;
    level = 0;
}

Player::Player(int x, int y) : Entity('@', x, y, 0, 0, "Player")
{
    floor = 0;
    health = 50;
    maxHealth = 50;
    attack = 10;
    defense = 5;
    coins = 0;
    points = 0;
    level = 0;
} 

void Player::playerSetup(Map& map)
{
    // find start Location on floor 0 in room 0
    std::pair<int, int> startLoc = map.floors[0].suitibleLocation(0);

    x = startLoc.first;
    y = startLoc.second;
}

std::string makeStatus(Player &player)
{
    // Health: actual(max) 4spaces Attack: num 4spaces Defense: num 4spaces coins: num.. points.. floor
    std::string status{};

    // Health stats
    status += "Health: " + std::to_string(player.getHealth()) + 
        "(" + std::to_string(player.getMaxHealth()) + ")    ";
    
    status += "Attack: " + std::to_string(player.getAttack()) + "    ";
    status += "Defense: " + std::to_string(player.getDefense()) + "    ";
    status += "Coins: " + std::to_string(player.getCoins()) + "    ";
    status += "Level: " + std::to_string(player.getLevel()) + "    ";
    status += "Floor: " + std::to_string(player.getFloor()) + "    ";

    return status;
}


/**
  * @param opponent
  * @param order 0 is player fight opponent 1 is opponent fight player
 */
void Player::combat(Entity &opponent, int order, std::function<void(std::string)> addMessage, std::function<void()> lose)
{
    if (order == 0)
    {
        int attackDamage = attack;
        if ((opponent.getHealth() - attackDamage) > 0)
        {
            opponent.changeHealth(-attackDamage);
            addMessage(opponent.getName() + " got attacked for " + std::to_string(attackDamage) + " by " + this->name);
        }
        else
        {
            opponent.changeHealth(-attackDamage);
            addMessage(this->name + " killed " + opponent.getName() + "!");

            // Get things when kill enemies
            points += 20;
            coins += 7;

            // Level up
            if (points / 75 > level)
            {
                level = points / 75;
                health = maxHealth;
                maxHealth += 20;
                attack += 2;
            }
        }
    }
    else
    {
        int attackDamage = opponent.getAttack();
        if ((health - attackDamage) > 0)
        {
            health -= attackDamage;
            addMessage(this->name + " got attacked for " + std::to_string(attackDamage) + " by " + opponent.getName());
        }
        else
        {
            health -= attackDamage;
            addMessage(opponent.getName() + " killed " + this->name + "!");
            lose();
        }
    }
}