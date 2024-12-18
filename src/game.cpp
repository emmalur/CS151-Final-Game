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

void checkPosition(int x, int y, Player &player, Level &level)
{
    switch(level.floor[y][x].symbol)
    {
        case ' ':
        case '#':
        case '.':
            player.move(x, y, level);
            break;
        case 'X':
        case 'Z':
        case 'G':
        case 'D':
            // combat(Player &player, Enemy &enemy)
            // do level.getEnemyAt(x, y) to get the enemy to put in combat
            break;
        default:
            break;
    }
}

void startMap(Player &player, Level &level, std::function<void()> quit, std::function<void()> back, std::function<void()> noMove)
{
    auto screen = ScreenInteractive::Fullscreen();
    auto exit = screen.ExitLoopClosure();

    static bool inventory = false;
    
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

    std::string inventoryMessage = "";

    if (inventory)
    {
        inventoryMessage = "You got stuff in your inventory probably";
        inventory = false;
    }

    // messages; like attacks maybe first help message
    page.push_back(
        text("Tip: Use wasd to move, press q to quit, b to go back to menu, and i to see inventory")
    );
    page.push_back(
        text(inventoryMessage)
    );

    // add all of the lines
    auto component = Renderer([&]
    {
        return vbox(page);        
    });

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
            noMove();
            exit();
            return true;
        }
        else if (event == Event::Character('w'))
        {
            exit();
            checkPosition(player.getX(), player.getY() - 1, player, level);
            return true;
        }
        else if (event == Event::Character('s'))
        {
            exit();
            checkPosition(player.getX(), player.getY() + 1, player, level);
            return true;
        }
        else if (event == Event::Character('d'))
        {
            exit();
            checkPosition(player.getX() + 1, player.getY(), player, level);
            return true;
        }
        else if (event == Event::Character('a'))
        {
            exit();
            checkPosition(player.getX() - 1, player.getY(), player, level);
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
    bool moved = true;
    // bool save = false;
    // bool load = false;

    // lambdas to give to functions so they can send up call stack
    auto on_quit = [&] { quit = true; };
    auto on_back = [&] { back = true; };
    auto on_noMove = [&] { moved = false; };

    startMainMenu(on_quit);

    // if (!quit || !save || !load)
    Map map(10);
    Player player = playerSetup(map);

    while (!quit)
    {
        startMap(player, map.floors[player.getFloor()], on_quit, on_back, on_noMove);

        if (back)
        {
            startMainMenu(on_quit);
            back = false;
        }

        // if the player moved, move the enemies
        if (moved)
        {
            map.floors[player.getFloor()].moveEnemies();
        }
    }
}
