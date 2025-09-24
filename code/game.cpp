#include "game.hpp"

Game::Game()
{
    rg::Init();
    display = &rg::display::SetMode(1024, 768);
    rg::display::SetCaption("Solitaire");
}

Game::~Game()
{
    rg::Quit();
}

void Game::run()
{
    while (!rg::WindowCloseOrQuit())
    {
        display->Fill(bg_color);
        rg::display::Update();
    }
}

