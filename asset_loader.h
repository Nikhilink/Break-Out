#pragma once

#include <unordered_map>
#include <string>

#include "raylib.h"

#include "game_objects.h"

class AssetLoader
{
    static AssetLoader* instance;

    static std::unordered_map<std::string, Texture2D> textures;
    static std::unordered_map<std::string, Sound> audios;
    static Music game_music;

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

        Texture2D getHearts();

        void PlayTrack(const char* sound_name);
        void PlayGameMusic();
        void PauseGameMusic();
        void StopGameMusic();
        void ResumeGameMusic();
        void UpdateGameMusic();
};

class GameAssetManager
{
    static GameAssetManager* instance;
    GameAssetManager() 
    {
        GenerateQuadsPaddles();  
        GenerateQuadsBalls();
        GenerateQuadsBricks();
        GenerateQuadsHearts();
    }
    public:

        static GameAssetManager* GetInstance();

        std::unordered_map<int, Rectangle> paddles;
        std::unordered_map<int, Rectangle> balls;
        std::unordered_map<int, Rectangle> bricks;
        std::unordered_map<int, Rectangle> hearts;

        Paddle paddle;
        Ball ball;
        int skin = 0;
        int size = 2;

        void GenerateQuadsPaddles();
        void GenerateQuadsBalls();
        void GenerateQuadsBricks();
        void GenerateQuadsHearts();

        std::unordered_map<int, Rectangle> GetPaddles();

        void SetSkin(int skin);

        static void CleanUp();
};