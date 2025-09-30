#pragma once
#include <rygame.hpp>


class Card : public rg::sprite::Sprite
{
public:

    Card(const std::string &suit, const std::string &value, rg::Surface *face_down);
    void face_up();
    void face_down();
    bool isRed() const;
    bool isBlack() const;
    int faceValue() const;
    bool isOtherColor(const Card *other) const;
    std::string suit{};
    std::string value{};
    bool isFaceUp{};

private:

    std::string filename{};
    rg::Surface *face_down_surf{};
    rg::Surface *face_up_surf{};
};
