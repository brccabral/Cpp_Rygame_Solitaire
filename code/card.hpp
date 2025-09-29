#pragma once
#include <rygame.hpp>


class Card : public rg::sprite::Sprite
{
public:

    Card(const std::string &suit, const std::string &value, rg::Surface *face_down);
    void face_up();
    void face_down();
    std::string suit{};
    std::string value{};

private:

    std::string filename{};
    bool isFaceUp{};
    rg::Surface *face_down_surf{};
    rg::Surface *face_up_surf{};
};
