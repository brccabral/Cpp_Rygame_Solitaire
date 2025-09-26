#include "card.hpp"

#include "resourcemanager.hpp"

Card::Card(const std::string &suit, const std::string &value, rg::Surface *face_down)
    : suit(suit), value(value), face_down(face_down)
{
    filename = "resources/cards/card" + suit + value + ".png";
    face_up = &ResourceManager::GetInstance().GetSurface(filename);
    image = face_down;
}
