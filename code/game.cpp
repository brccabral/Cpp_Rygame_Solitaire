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
        if (rl::IsMouseButtonReleased(rl::MOUSE_BUTTON_LEFT))
        {
            OnMouseRelease();
        }
        const auto [dx, dy] = rl::GetMouseDelta();
        if (dx != 0 || dy != 0)
        {
            OnMouseMotion(dx, dy);
        }

        display.Fill(bg_color);

        for (const auto &mat: pile_mat_list)
        {
            rg::draw::rect(&display, mat_color, mat);
        }

        card_list.Draw(&display);

        screen->Blit(&display, rg::math::Vector2<float>{}, rl::BLEND_ALPHA, CARD_SCALE);
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

    pile_mat_list.emplace_back(START_X, static_cast<float>(BOTTOM_Y), MAT_WIDTH, MAT_HEIGHT);
    pile_mat_list.emplace_back(START_X + X_SPACING, static_cast<float>(BOTTOM_Y), MAT_WIDTH, MAT_HEIGHT);
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
    held_cards.clear();
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
        card.rect.center(pile_mat_list[BOTTOM_FACE_DOWN_PILE].center());
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
            card->rect.center(pile_mat_list[pile_no].center());
            // put it in the top of drawing group
            PullToTop(card);
        }
    }

    // show card faces on top card of each playing pile
    for (int pile_no = PLAY_PILE_1; pile_no < PLAY_PILE_7 + 1; ++pile_no)
    {
        auto *card_sprite = piles[pile_no].back();
        auto *card = dynamic_cast<Card *>(card_sprite);
        card->face_up();
    }

}

void Game::PullToTop(rg::sprite::Sprite *card)
{
    card_list.remove(card);
    card_list.add(card);
}

void Game::OnMousePress(const int x, const int y)
{
    const auto clicked = rg::sprite::pointcollide(
            {static_cast<float>(x), static_cast<float>(y)}, &card_list, false);
    if (!clicked.empty())
    {
        auto *primary_card = dynamic_cast<Card *>(clicked.back());
        const auto pile_index = GetPileForCard(primary_card);

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
                card->rect.center(pile_mat_list[BOTTOM_FACE_UP_PILE].center());
                piles[BOTTOM_FACE_DOWN_PILE].remove(card);
                piles[BOTTOM_FACE_UP_PILE].add(card);
                PullToTop(card);
            }
        }
        else if (piles[pile_index].back() == primary_card &&
                 !primary_card->isFaceUp)
        {
            primary_card->face_up();
        }
        else if (primary_card->isFaceUp)
        {
            held_cards.emplace_back(primary_card);
            held_cards_original_pos.emplace_back(primary_card->rect);
            PullToTop(primary_card);

            const auto card_index = piles[pile_index].index(primary_card);
            for (int i = card_index + 1; i < piles[pile_index].size(); ++i)
            {
                auto *card = dynamic_cast<Card *>(piles[pile_index][i]);
                held_cards.emplace_back(card);
                held_cards_original_pos.emplace_back(card->rect);
                PullToTop(card);
            }
        }
    }
    else
    {
        if (rl::CheckCollisionPointRec(
                {(float) x, (float) y}, pile_mat_list[BOTTOM_FACE_DOWN_PILE].rectangle()))
        {
            auto tmp_list = piles[BOTTOM_FACE_UP_PILE].Sprites();
            const std::ranges::reverse_view rev{tmp_list};
            for (auto *card_sprite: rev)
            {
                auto *card = dynamic_cast<Card *>(card_sprite);
                card->face_down();
                piles[BOTTOM_FACE_UP_PILE].remove(card);
                piles[BOTTOM_FACE_DOWN_PILE].add(card);
                card->rect.center(pile_mat_list[BOTTOM_FACE_DOWN_PILE].center());
                PullToTop(card);
            }
        }
    }
}

void Game::OnMouseRelease()
{
    if (held_cards.empty())
    {
        return;
    }

    auto *held_card = held_cards.front();
    const auto collided = rg::sprite::spritecollide(held_card, &card_list, false);
    // will always collide with self
    const auto collided_index = rg::index(collided, dynamic_cast<rg::sprite::Sprite *>(held_card));
    if (collided_index > 0)
    {
        auto *target = dynamic_cast<Card *>(collided[collided_index - 1]);
        if (target->isFaceUp)
        {
            const auto pile_index = GetPileForCard(target);
            if (pile_index >= PLAY_PILE_1 && pile_index <= PLAY_PILE_7)
            {
                if (held_card->isOtherColor(target)
                    && held_card->faceValue() == target->faceValue() - 1
                )
                {
                    auto target_rect = target->rect;
                    for (auto *card: held_cards)
                    {
                        MoveCardToPile(card, pile_index);
                        card->rect = target_rect;
                        card->rect.y += CARD_VERTICAL_OFFSET;
                        target_rect = card->rect;
                    }
                    goto clear;
                }
            }
            else if (pile_index >= TOP_PILE_1 && pile_index <= TOP_PILE_4)
            {
                if (held_card->suit == target->suit
                    && held_card->faceValue() - 1 == target->faceValue()
                )
                {
                    MoveCardToPile(held_card, pile_index);
                    held_card->rect = target->rect;
                    goto clear;
                }
            }
        }
    }
    else if (const auto mat_index = held_card->rect.collidelist(pile_mat_list);
        mat_index > BOTTOM_FACE_UP_PILE)
    {
        if (mat_index >= TOP_PILE_1
            && mat_index <= TOP_PILE_4
            && held_card->faceValue() == 1
            && piles[mat_index].size() == 0
        )
        {
            MoveCardToPile(held_card, mat_index);
            held_card->rect.center(pile_mat_list[mat_index].center());
            goto clear;
        }
        if (mat_index >= PLAY_PILE_1
            && mat_index <= PLAY_PILE_7
            && held_card->faceValue() == 13
            && piles[mat_index].size() == 0
        )
        {
            auto target_rect = pile_mat_list[mat_index];
            for (auto *card: held_cards)
            {
                MoveCardToPile(card, mat_index);
                card->rect.center(target_rect.center());
                if (card != held_card)
                {
                    card->rect.y += CARD_VERTICAL_OFFSET;
                }
                target_rect = card->rect;
            }
            goto clear;
        }
    }
    for (int h = 0; h < held_cards.size(); ++h)
    {
        held_cards[h]->rect = held_cards_original_pos[h];
    }
clear:
    held_cards.clear();
    held_cards_original_pos.clear();
}

void Game::OnMouseMotion(const float dx, const float dy) const
{
    for (auto *card: held_cards)
    {
        card->rect.x += dx / CARD_SCALE;
        card->rect.y += dy / CARD_SCALE;
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

void Game::MoveCardToPile(Card *card, const PileIndex pile_index)
{
    RemoveCardFromAnyPile(card);
    piles[pile_index].add(card);
}

void Game::RemoveCardFromAnyPile(Card *card)
{
    for (auto &pile: piles | std::views::values)
    {
        pile.remove(card);
    }
}

