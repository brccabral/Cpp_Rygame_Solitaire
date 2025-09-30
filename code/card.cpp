#include "card.hpp"

#include "resourcemanager.hpp"

Card::Card(const std::string &suit, const std::string &value, rg::Surface *face_down)
    : suit(suit), value(value), face_down_surf(face_down)
{
    filename = "resources/cards/card" + suit + value + ".png";
    face_up_surf = &ResourceManager::GetInstance().GetSurface(filename);
    image = face_down_surf;
}

void Card::face_up()
{
    image = face_up_surf;
    isFaceUp = true;
}

void Card::face_down()
{
    image = face_down_surf;
    isFaceUp = false;
}

bool Card::isRed() const
{
    return suit == "Diamonds" || suit == "Hearts";
}

bool Card::isBlack() const
{
    return suit == "Clubs" || suit == "Spades";
}

int Card::faceValue() const
{
    if (value == "A")
    {
        return 1;
    }
    if (value == "2")
    {
        return 2;
    }
    if (value == "3")
    {
        return 3;
    }
    if (value == "4")
    {
        return 4;
    }
    if (value == "5")
    {
        return 5;
    }
    if (value == "6")
    {
        return 6;
    }
    if (value == "7")
    {
        return 7;
    }
    if (value == "8")
    {
        return 8;
    }
    if (value == "9")
    {
        return 9;
    }
    if (value == "10")
    {
        return 10;
    }
    if (value == "J")
    {
        return 11;
    }
    if (value == "Q")
    {
        return 12;
    }
    if (value == "K")
    {
        return 13;
    }
    return 0;
}

bool Card::isOtherColor(const Card *other) const
{
    return isRed() && other->isBlack() || isBlack() && other->isRed();
}

