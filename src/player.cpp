#include "player.h"

#include <string>

Player::Player(int x, int y) : Entity('@', x, y)
{
    floor = 0;
    health = 20;
    maxHealth = 20;
    attack = 5;
    defense = 5;
    coins = 0;
} 

void Player::moveVertical(int direction, Level &level)
{
    if (direction > 0)
    {
        // Top of map is y = 0 so subtracting moves up
        if (level.isMoveable(x, y - 1))
            y--;
    }
    else if (direction < 0)
    {
        if (level.isMoveable(x, y + 1))
            y++;
    }
}

void Player::moveHorizontal(int direction, Level &level)
{
    if (direction > 0)
    {
        if (level.isMoveable(x + 1, y))
            x++;
    }
    else if (direction < 0)
    {
        if (level.isMoveable(x - 1, y))
            x--;
    }
}

Player playerSetup(Map& map)
{
    // find start Location on floor 0 in room 0
    std::pair<int, int> startLoc = map.floors[0].suitibleLocation(0);

    return Player(startLoc.first, startLoc.second);
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
    status += "Floor: " + std::to_string(player.getFloor()) + "    ";

    return status;
}