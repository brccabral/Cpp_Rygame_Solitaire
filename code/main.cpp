#include "game.hpp"

int main()
{
    rg::Init();
    {
        Game game;
        game.run();
    }
    rg::Quit();
    return 0;
}
