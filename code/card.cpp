#include "card.hpp"

#include "resourcemanager.hpp"

Card::Card(const std::string &suit, const std::string &value)
    : suit(suit), value(value)
{
    filename = "resources/cards/card" + suit + value + ".png";
    image = &ResourceManager::GetInstance().GetSurface(filename);
}
