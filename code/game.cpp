#include "game.hpp"
#include "resourcemanager.hpp"


Game::Game()
{
    rg::Init();
    screen = &rg::display::SetMode(1024, 768);
    display = rg::Surface(1706, 1280);
    rg::display::SetCaption("Solitaire");
    LoadCards();
    Reset();
}

Game::~Game()
{
    ResourceManager::Destroy();
    display.~Surface();
    rg::Quit();
}

void Game::run()
{
    while (!rg::WindowCloseOrQuit())
    {
        display.Fill(bg_color);

        for (const auto &mat: pile_mat_list)
        {
            rg::draw::rect(&display, mat_color, mat);
        }

        // cards_group.Draw(&display);

        screen->Blit(&display, rg::math::Vector2{}, rl::BLEND_ALPHA, CARD_SCALE);
        rg::display::Update();
    }
}

void Game::LoadCards()
{
    cards.reserve(53);
    cards.emplace_back("Back_", "red2");
    std::array<std::string, 4> suits = {"Clubs", "Diamonds", "Hearts", "Spades"};
    std::array<std::string, 13> values = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q",
                                          "K", "A"};
    for (auto &suit: suits)
    {
        for (auto &value: values)
        {
            cards.emplace_back(suit, value);
        }
    }

    pile_mat_list.emplace_back(START_X, BOTTOM_Y, MAT_WIDTH, MAT_HEIGHT);
    pile_mat_list.emplace_back(START_X + X_SPACING, BOTTOM_Y, MAT_WIDTH, MAT_HEIGHT);
    for (int i = 0; i < 7; ++i)
    {
        pile_mat_list.emplace_back(START_X + i * X_SPACING, MIDDLE_Y, MAT_WIDTH, MAT_HEIGHT);
    }

    for (int i = 0; i < 4; ++i)
    {
        pile_mat_list.emplace_back(START_X + i * X_SPACING, TOP_Y, MAT_WIDTH, MAT_HEIGHT);
    }
}

void Game::Reset()
{
    held_cards.empty();
    held_cards_original_pos.clear();

    int n_cols = 8;
    int n_rows = 7;

    for (int r = 0; r < n_rows; r++)
    {
        for (int c = 0; c < n_cols; ++c)
        {
            const int index = r * n_cols + c;
            if (index > cards.size() - 1)
            {
                return;
            }
            Card &card = cards[index];
            card.rect = {140.0f * c, 190.0f * r, 140.0f, 190.0f};
            card.add(&cards_group);
        }
    }
}

