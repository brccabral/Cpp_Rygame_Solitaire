#pragma once
#include <rygame.hpp>


class Card : public rg::sprite::Sprite
{
public:

    Card(const std::string &suit, const std::string &value);

private:

    std::string suit{};
    std::string value{};
    std::string filename{};
};
