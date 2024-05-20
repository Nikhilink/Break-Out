#pragma once

#include <unordered_map>
#include <string>

#include "raylib.h"

#include "game_objects.h"

class AssetLoader
{
    static AssetLoader* instance;

    static std::unordered_map<std::string, Texture2D> textures;

    AssetLoader(){}

    static void loadData();

    public:
        /**
         * Initializes the Instance and Loads all the Assets
        */
        static AssetLoader* getInstance();

        /**
         * Deletes the Instance hence removes all of the assets
        */
        static void destroyInstance();
    
        Texture2D getBackground();

        Texture2D getBlocks();
};

class GameAssetManager
{
    static GameAssetManager* instance;
    GameAssetManager() 
    {
        GenerateQuadsPaddles();  
        GenerateQuadsBalls();
        GenerateQuadsBricks();
    }
    public:

        static GameAssetManager* GetInstance();

        std::unordered_map<int, Rectangle> paddles;
        std::unordered_map<int, Rectangle> balls;
        std::unordered_map<int, Rectangle> bricks;

        Paddle paddle;
        Ball ball;
        int skin = 0;
        int size = 2;

        void GenerateQuadsPaddles();
        void GenerateQuadsBalls();
        void GenerateQuadsBricks();

        std::unordered_map<int, Rectangle> GetPaddles();

        void SetSkin(int skin);

        static void CleanUp();
};