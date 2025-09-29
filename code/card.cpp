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
