#pragma once
#include <memory>
#include <unordered_map>

#include <rygame.hpp>


class ResourceManager
{
public:

    static ResourceManager &GetInstance();
    static void Destroy();

    rg::Surface &GetSurface(const std::string &filename);

private:

    ResourceManager() = default;

    inline static std::unique_ptr<ResourceManager> instance;
    std::unordered_map<std::string, rg::Surface> surfaces;
};
