#pragma once
#include <rygame.hpp>
#include "card.hpp"


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define CARD_SCALE 0.6f
#define CARD_WIDTH 140
#define CARD_HEIGHT 190
#define MAT_PERCENT_OVERSIZE 1.25
#define MAT_HEIGHT (CARD_HEIGHT * MAT_PERCENT_OVERSIZE)
#define MAT_WIDTH (CARD_WIDTH * MAT_PERCENT_OVERSIZE)
#define MAT_OFFSET (MAT_WIDTH * 0.1)
#define VERTICAL_MARGIN_PERCENT 0.1f
#define HORIZONTAL_MARGIN_PERCENT 0.1f
#define BOTTOM_Y (SCREEN_HEIGHT + CARD_HEIGHT)
#define START_X (MAT_WIDTH * HORIZONTAL_MARGIN_PERCENT)
#define TOP_Y MAT_HEIGHT * VERTICAL_MARGIN_PERCENT
#define MIDDLE_Y (TOP_Y + MAT_HEIGHT + MAT_HEIGHT * VERTICAL_MARGIN_PERCENT)
#define X_SPACING (MAT_WIDTH + MAT_WIDTH * HORIZONTAL_MARGIN_PERCENT)
#define CARD_VERTICAL_OFFSET (CARD_HEIGHT * CARD_SCALE * 0.3f)
#define BOTTOM_FACE_DOWN_PILE 0
#define BOTTOM_FACE_UP_PILE 1
#define PLAY_PILE_1 2
#define PLAY_PILE_2 3
#define PLAY_PILE_3 4
#define PLAY_PILE_4 5
#define PLAY_PILE_5 6
#define PLAY_PILE_6 7
#define PLAY_PILE_7 8
#define TOP_PILE_1 9
#define TOP_PILE_2 10
#define TOP_PILE_3 11
#define TOP_PILE_4 12
#define FACE_DOWN_FILENAME "resources/cards/cardBack_red2.png"
#define PILE_COUNT 13
#define FLIP_CARDS 1

typedef int PileIndex;

class Game
{
public:

    Game();
    ~Game();

    void run();

private:

    void LoadCards();
    void Reset();
    void PullToTop(rg::sprite::Sprite *card);
    void OnMousePress(int x, int y);
    void OnMouseRelease();
    void OnMouseMotion(float dx, float dy) const;
    PileIndex GetPileForCard(Card *);
    void MoveCardToPile(Card *card, PileIndex pile_index);
    void RemoveCardFromAnyPile(Card *card);

    rg::Surface *screen{};
    rg::Surface display{};
    rl::Color bg_color = rl::Color{59, 122, 87, 255};
    rl::Color mat_color = rl::Color{85, 107, 47, 255};

    rg::Surface *face_down{};
    std::vector<Card> cards{};
    // holds the drawing order (all cards should be here)
    rg::sprite::OrderedUpdates card_list{};
    std::vector<Card *> held_cards{};
    std::vector<rg::Rect> held_cards_original_pos{};
    std::vector<rg::Rect> pile_mat_list{};
    std::unordered_map<PileIndex, rg::sprite::OrderedUpdates> piles{};
};
