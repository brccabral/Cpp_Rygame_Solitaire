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
        if (rl::IsKeyPressed(rl::KEY_R))
        {
            Reset();
        }

        if (rl::IsMouseButtonPressed(rl::MOUSE_BUTTON_LEFT))
        {
            OnMousePress(rl::GetMouseX() / CARD_SCALE, rl::GetMouseY() / CARD_SCALE);
        }

        display.Fill(bg_color);

        for (const auto &mat: pile_mat_list)
        {
            rg::draw::rect(&display, mat_color, mat);
        }

        card_list.Draw(&display);

        screen->Blit(&display, rg::math::Vector2{}, rl::BLEND_ALPHA, CARD_SCALE);
        rg::display::Update();
    }
}

void Game::LoadCards()
{
    face_down = &ResourceManager::GetInstance().GetSurface(FACE_DOWN_FILENAME);
    cards.reserve(52);
    std::array<std::string, 4> suits = {"Clubs", "Diamonds", "Hearts", "Spades"};
    std::array<std::string, 13> values = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q",
                                          "K", "A"};
    for (auto &suit: suits)
    {
        for (auto &value: values)
        {
            cards.emplace_back(suit, value, face_down);
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
    for (auto &pile: piles | std::views::values)
    {
        pile.empty();
    }
    piles.clear();

    // resets drawing order
    card_list.empty();
    for (auto &card: cards)
    {
        card.rect = {START_X + MAT_OFFSET, BOTTOM_Y, CARD_WIDTH, CARD_HEIGHT};
        card_list.add(&card);
        card.face_down();
    }

    // shuffles the deck
    std::ranges::shuffle(cards, *rg::math::get_rng());

    // put all cards in dealing pile
    for (auto &card: cards)
    {
        piles[BOTTOM_FACE_DOWN_PILE].add(&card);
    }

    // distribute cards in playing piles
    for (int pile_no = PLAY_PILE_1; pile_no < PLAY_PILE_7 + 1; ++pile_no)
    {
        for (int j = 0; j < pile_no - PLAY_PILE_1 + 1; ++j)
        {
            // remove from dealing pile
            auto *card = piles[BOTTOM_FACE_DOWN_PILE].pop();
            // add to current pile
            piles[pile_no].add(card);
            // set drawing position
            card->rect = pile_mat_list[pile_no];
            card->rect.x += MAT_OFFSET;
            // put it in the top of drawing group
            PullToTop(card);
        }
    }

    // show card faces on top card of each playing pile
    for (int pile_no = PLAY_PILE_1; pile_no < PLAY_PILE_7 + 1; ++pile_no)
    {
        auto *card_sprite = piles[pile_no].back();
        auto *card = dynamic_cast<Card *>(card_sprite);
        card->rect.y += CARD_VERTICAL_OFFSET;
        card->face_up();
    }

}

void Game::PullToTop(rg::sprite::Sprite *card)
{
    card_list.remove(card);
    card_list.add(card);
}

void Game::OnMousePress(int x, int y)
{
    auto clicked = rg::sprite::pointcollide({x, y}, &card_list, false);
    if (!clicked.empty())
    {
        auto *primary_card = dynamic_cast<Card *>(clicked.back());
        auto pile_index = GetPileForCard(primary_card);

        if (pile_index == BOTTOM_FACE_DOWN_PILE)
        {
            for (int i = 0; i < FLIP_CARDS; ++i)
            {
                if (piles[BOTTOM_FACE_DOWN_PILE].size() == 0)
                {
                    break;
                }
                auto *card = dynamic_cast<Card *>(piles[BOTTOM_FACE_DOWN_PILE].back());
                card->face_up();
                card->rect = pile_mat_list[BOTTOM_FACE_UP_PILE];
                card->rect.x += MAT_OFFSET;
                piles[BOTTOM_FACE_DOWN_PILE].remove(card);
                piles[BOTTOM_FACE_UP_PILE].add(card);
                PullToTop(card);
            }
        }
        else if (piles[pile_index].size() == 1 &&
                 !primary_card->isFaceUp)
        {
            primary_card->face_up();
        }
    }
}

PileIndex Game::GetPileForCard(Card *card)
{
    for (auto &[index, pile]: piles)
    {
        if (pile.has(card))
        {
            return index;
        }
    }
    return -1;
}

