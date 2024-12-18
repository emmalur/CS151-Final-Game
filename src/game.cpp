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

void startMainMenu()
{
    auto screen = ScreenInteractive::Fullscreen();
    auto exit = screen.ExitLoopClosure();

    auto play_and_exit = [exit]() 
    {
        exit();
    };

    auto menu = mainMenu(play_and_exit);
    screen.Loop(menu);
}

void startMap(Player &player, Level &level, std::function<void()> quit)
{
    auto screen = ScreenInteractive::Fullscreen();
    auto exit = screen.ExitLoopClosure();
    
    auto quit_and_exit = [quit, exit]()
    {
        quit();
        exit();
    };

    std::vector<Entity> entities{};
    entities.push_back(player);

    Elements page = cellsToElements(level.floor, entities);

    // player status
    std::string playerStatus = makeStatus(player);
    page.push_back(
        vbox({
            text(playerStatus) | bgcolor(Color::Blue) | flex
        })
    );

    // messages; like attacks maybe first help message
    page.push_back(
        text("Tip: Use wasd to move, press b to go back to menu, ...") | bgcolor(Color::Black) | flex
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
        else if (event == Event::Character('w'))
        {
            exit();
            player.moveVertical(1, level);
            return true;
        }
        else if (event == Event::Character('s'))
        {
            exit();
            player.moveVertical(-1, level);
            return true;
        }
        else if (event == Event::Character('d'))
        {
            exit();
            player.moveHorizontal(1, level);
            return true;
        }
        else if (event == Event::Character('a'))
        {
            exit();
            player.moveHorizontal(-1, level);
            return true;
        }
        return false;
    });

    screen.Loop(component);
}

void startGame()
{
    bool quit = false;
    // lambda to give to functions so they can say we quit
    auto on_quit = [&] { quit = true; };

    Map map(10);
    Player player = playerSetup(map);

    startMainMenu();
    while (!quit)
    {
        startMap(player, map.floors[player.getFloor()], on_quit);
    }
}
