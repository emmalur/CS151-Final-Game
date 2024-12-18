#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <functional>

/**
 * contains game loop and setup
 */
void startGame();

ftxui::Component mainMenu(std::function<void()> play, std::function<void()> quit);
