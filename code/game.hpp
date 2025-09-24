#pragma once
#include <rygame.hpp>

class Game
{
public:

    Game();
    ~Game();

    void run();

private:

    rg::Surface *display;
    rl::Color bg_color = rl::Color{59, 122, 87, 255};
};
