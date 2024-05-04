#include "asset_loader.h"

#pragma region Asset Loader
std::unordered_map<std::string, Texture2D> AssetLoader::textures;
AssetLoader* AssetLoader::instance;

void AssetLoader::loadData()
{
    textures["background"] = LoadTexture("C:/Users/nikhi/Documents/nikhil/projects/cpp/Break Out/assets/images/background.png");

    textures["blocks"] = LoadTexture("C:/Users/nikhi/Documents/nikhil/projects/cpp/Break Out/assets/images/blocks.png");
}

AssetLoader* AssetLoader::getInstance()
{
    if(!instance)
    {
        instance = new AssetLoader();
        loadData();
    }
    return instance;
}

Texture2D AssetLoader::getBackground()
{
    return textures["background"];
}

Texture2D AssetLoader::getBlocks()
{
    return textures["blocks"];
}

void AssetLoader::destroyInstance()
{
    TraceLog(LOG_INFO, "Cleaned Asset Loader");
    delete instance;
}

#pragma endregion
#pragma region Game Asset Manager

GameAssetManager* GameAssetManager::instance;

GameAssetManager* GameAssetManager::GetInstance()
{
    if(!instance)
    {
        instance = new GameAssetManager();
    }
    return instance;
}

void GameAssetManager::GenerateQuadsPaddles()
{
    float x = 0;
    float y = 64;

    int counter = 0;

    for(int i = 0;i < 4;i++)
    {
        paddles[counter] = {x, y, 32, 16};
        counter++;

        paddles[counter] = {x + 32, y, 64, 16};
        counter++;

        paddles[counter] = {x + 96, y, 96, 16};
        counter++;

        paddles[counter] = {x, y + 16, 128, 16};
        counter++;

        x = 0;
        y += 32;
    }
}

std::unordered_map<int, Rectangle> GameAssetManager::GetPaddles()
{
    return paddles;
}
void GameAssetManager::SetSkin(int skin)
{
    this->skin = skin;
}

void GameAssetManager::GenerateQuadsBalls()
{
    float x = 96;
    float y = 48;

    int counter = 0;

    for(int i = 0;i < 4;i++)
    {
        balls[counter] = {x, y, 8, 8};
        x += 8;
        counter += 1;
    }

    x = 96;
    y = 56;

    for(int i = 0;i < 3;i++)
    {
        balls[counter] = {x, y, 8, 8};
        x += 8;
        counter += 1;
    }
}

void GameAssetManager::CleanUp()
{
    TraceLog(LOG_INFO, "Cleaning Game Asset Manager");
    delete instance;
}

#pragma endregion