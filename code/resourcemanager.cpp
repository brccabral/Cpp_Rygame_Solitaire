#include "resourcemanager.hpp"

ResourceManager &ResourceManager::GetInstance()
{
    if (!instance)
    {
        instance = new ResourceManager();
    }
    return *instance;
}

void ResourceManager::Destroy()
{
    delete instance;
}

rg::Surface &ResourceManager::GetSurface(const std::string &filename)
{
    const auto it = surfaces.find(filename);
    if (it != surfaces.end())
    {
        return it->second;
    }
    surfaces.emplace(filename, rg::image::Load(filename.c_str()));
    return surfaces.at(filename);
}

ResourceManager *ResourceManager::instance = nullptr;
