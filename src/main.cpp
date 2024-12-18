#include <iostream>

// Needs if there is no tests defined
#define DOCTEST_CONFIG_DISABLE
//#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include "nlohmann/json.hpp"

#include "game.h"

int main()
{
    // these two lines are for unit testing
    // doctest::Context context;
    // context.run();

    startGame();

    return 0;
}
