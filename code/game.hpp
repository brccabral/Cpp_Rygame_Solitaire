#pragma once
#include <rygame.hpp>

#include "card.hpp"

class Game
{
public:

    Game();
    ~Game();

    void run();

private:

    void LoadCards();
    void Reset();

    rg::Surface *screen{};
    rg::Surface display{};
    rl::Color bg_color = rl::Color{59, 122, 87, 255};

    std::vector<Card> cards{};
    rg::sprite::Group cards_group{};
};
