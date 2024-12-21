#include "game.h"
#include "player.h"
#include "map.h"

#include <iostream>
#include <vector>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

void startMainMenu(std::function<void()> quit)
{
    auto screen = ScreenInteractive::Fullscreen();
    auto exit = screen.ExitLoopClosure();

    auto quit_and_exit = [quit, exit]() 
    {
        quit();
        exit();
    };

    auto menu = mainMenu(exit, quit_and_exit);
    screen.Loop(menu);
}

void checkPosition(int x, int y, Player &player, Level &level, std::function<void(std::string)> addMessage, std::function<void()> lose)
{
    if (level.isMoveable(x, y) != Moveable::Enemy)
    {
        player.move(x, y, level);
    }
    else
    {
        player.combat(level.getEnemyAt(x, y), 0, addMessage, lose);
    }
}

void startMap(Player &player, Level &level, std::function<void()> quit, std::function<void()> back,
            std::function<void()> floorChange, std::function<void()> lose, std::function<void()> win,
            std::function<void(std::string)> addMessage, std::function<void()> removeMessage, 
            const std::vector<std::string> &messages)
{
    auto screen = ScreenInteractive::Fullscreen();
    auto exit = screen.ExitLoopClosure();

    static bool inventory = false;
    static bool tip = true;
    
    auto quit_and_exit = [quit, exit]()
    {
        quit();
        exit();
    };

    auto back_and_exit = [back, exit]()
    {
        back();
        exit();
    };

    /** Display game **/

    std::vector<Entity> entities{};

    // Add all entities to get drawn
    entities.push_back(player);
    for (Enemy enemy : level.enemies)
    {
        entities.push_back(enemy);
    }

    Elements page = cellsToElements(level.floor, entities);

    // player status
    std::string playerStatus = makeStatus(player);
    page.push_back(
        vbox({
            text(playerStatus) | bgcolor(Color::Blue) | flex
        })
    );

    if (tip)
    {
        addMessage("Tip: Use wasd to move, press q to quit, b to go back to menu, and i to see inventory, press . to go down stairs");
        tip = false;
    }

    std::string inventoryMessage = "";

    if (inventory)
    {
        inventoryMessage = "You got stuff in your inventory probably";
        addMessage(inventoryMessage);
        inventory = false;
    }

    // Get rid of old messages
    if (messages.size() > 15)
    {
        removeMessage();
    }

    for (int i = static_cast<int>(messages.size()); i > 0; i--)
    {
        page.push_back(text(messages[i - 1]));
    }   

    auto component = Renderer([&]
    {
        return vbox(page);        
    });

    /** Handle Input **/

    component |= CatchEvent([&](Event event)
    {
        if (event == Event::Character('q'))
        {
            quit_and_exit();
            return true;
        }
        else if (event == Event::Character('b'))
        {
            back_and_exit();
            return true;
        }
        else if (event == Event::Character('i'))
        {
            inventory = true;
            exit();
            return true;
        }
        else if (event == Event::Character('w'))
        {
            exit();
            checkPosition(player.getX(), player.getY() - 1, player, level, addMessage, lose);
            return true;
        }
        else if (event == Event::Character('s'))
        {
            exit();
            checkPosition(player.getX(), player.getY() + 1, player, level, addMessage, lose);
            return true;
        }
        else if (event == Event::Character('d'))
        {
            exit();
            checkPosition(player.getX() + 1, player.getY(), player, level, addMessage, lose);
            return true;
        }
        else if (event == Event::Character('a'))
        {
            exit();
            checkPosition(player.getX() - 1, player.getY(), player, level, addMessage, lose);
            return true;
        }
        else if (event == Event::Character('.'))
        {
            exit();
            if (level.floor[player.getY()][player.getX()].symbol == '>')
            {
                if (player.getFloor() == 9)
                {
                    win();
                }
                else
                {
                    player.moveFloor(1);
                    floorChange();
                }
            }
            return true;
        }
        return false;
    });

    screen.Loop(component);
}

void startGame()
{
    bool quit = false;
    bool back = false;
    bool moved = false;
    bool lost = false;
    bool won = false;
    // bool save = false;
    // bool load = false;

    // Messages to display at bottom of screen
    std::vector<std::string> messages{};

    auto addMessage = [&messages](std::string message)
    {
        messages.push_back(message);
    };

    auto removeMessage = [&messages]()
    {
        messages.erase(messages.begin());
    };

    // lambdas to give to functions so they can send up call stack
    auto on_quit = [&] { quit = true; };
    auto on_back = [&] { back = true; };
    auto on_floorChange = [&] { moved = true; };
    auto on_lose = [&] { lost = true; };
    auto on_win = [&] { won = true; };

    startMainMenu(on_quit);

    // if (!quit || !save || !load)
    Player player;
    Map map(10, player);
    player.playerSetup(map);

    while (!quit)
    {
        // player changed floors
        if (moved)
        {
            std::pair newStartLoc = map.floors[player.getFloor()].suitibleLocation(0);
            player.move(newStartLoc.first, newStartLoc.second, map.floors[player.getFloor()]);
            moved = false;
        }

        startMap(player, map.floors[player.getFloor()], on_quit, on_back, on_floorChange, on_lose, on_win, addMessage, removeMessage, messages);

        if (lost)
        {
            std::cout << "Sorry you lost the game, You got " << player.getPoints() << " Points!!\n";
            return;
        }

        if (won)
        {
            std::cout << "You won, you got " << player.getPoints() << " Points!!\n";
            return;
        }

        if (back)
        {
            startMainMenu(on_quit);
            back = false;
        }

        map.floors[player.getFloor()].moveEnemies(addMessage, on_lose);
    }
}
