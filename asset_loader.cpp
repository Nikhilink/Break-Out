#include "asset_loader.h"

#pragma region Asset Loader
std::unordered_map<std::string, Texture2D> AssetLoader::textures;
std::unordered_map<std::string, Sound> AssetLoader::audios;
Music AssetLoader::game_music;
AssetLoader* AssetLoader::instance;

void AssetLoader::loadData()
{
    textures["background"] = LoadTexture("assets/images/background.png");

    textures["blocks"] = LoadTexture("assets/images/blocks.png");
    textures["hearts"] = LoadTexture("assets/images/hearts.png");

    audios["brick-hit-1"] = LoadSound("assets/sounds/brick-hit-1.wav");
    audios["brick-hit-2"] = LoadSound("assets/sounds/brick-hit-2.wav");
    audios["confirm"] = LoadSound("assets/sounds/confirm.wav");
    audios["highscore"] = LoadSound("assets/sounds/high_score.wav");
    audios["hurt"] = LoadSound("assets/sounds/hurt.wav");
    // audios["music"] = LoadSound("assets/sounds/music.wav");
    audios["no-select"] = LoadSound("assets/sounds/no-select.wav");
    audios["paddle-hit"] = LoadSound("assets/sounds/paddle_hit.wav");
    audios["pause"] = LoadSound("assets/sounds/pause.wav");
    audios["recover"] = LoadSound("assets/sounds/recover.wav");
    audios["select"] = LoadSound("assets/sounds/select.wav");
    audios["score"] = LoadSound("assets/sounds/score.wav");
    audios["victory"] = LoadSound("assets/sounds/victory.wav");
    audios["wall_hit"] = LoadSound("assets/sounds/wall_hit.wav");

    game_music = LoadMusicStream("assets/sounds/music.wav");
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
void AssetLoader::PlayGameMusic()
{
    PlayMusicStream(game_music);
}
void AssetLoader::StopGameMusic()
{
    StopMusicStream(game_music);
}
void AssetLoader::PauseGameMusic()
{
    PauseMusicStream(game_music);
}
void AssetLoader::ResumeGameMusic()
{
    ResumeMusicStream(game_music);
}
void AssetLoader::PlayTrack(const char* sound_name)
{
    PlaySound(audios[sound_name]);
}
void AssetLoader::UpdateGameMusic()
{
    UpdateMusicStream(game_music);
}

Texture2D AssetLoader::getBackground()
{
    return textures["background"];
}
Texture2D AssetLoader::getHearts()
{
    return textures["hearts"];
}

Texture2D AssetLoader::getBlocks()
{
    return textures["blocks"];
}

void AssetLoader::destroyInstance()
{
    UnloadTexture(textures["background"]);

    UnloadTexture(textures["blocks"]);
    UnloadTexture(textures["hearts"]);

    UnloadSound(audios["brick-hit-1"]);
    UnloadSound(audios["brick-hit-2"]);
    UnloadSound(audios["confirm"]);
    UnloadSound(audios["highscore"]);
    UnloadSound(audios["hurt"]);
    UnloadSound(audios["no-select"]);
    UnloadSound(audios["paddle-hit"]);
    UnloadSound(audios["pause"]);
    UnloadSound(audios["recover"]);
    UnloadSound(audios["select"]);
    UnloadSound(audios["score"]);
    UnloadSound(audios["victory"]);
    UnloadSound(audios["wall_hit"]);
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
void GameAssetManager::GenerateQuadsHearts()
{
    float x = 0;
    float y = 0;

    hearts[0] = {x, y, 10, 9};

    x += 10;

    hearts[1] = {x, y, 10, 9};
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

void GameAssetManager::GenerateQuadsBricks()
{
    float x = 0;
    float y = 0;

    int counter = 0;

    for(int j = 0; j <= 16*2;j+=16)
    {
        for(int i = 0;i <= 32 * 5;i+=32)
        {
            x = i;
            y = j;
            bricks[counter++] = {x, y, 32, 16};
        }
    }

    // for(int i = 0;i <= 32 * 5;i+=32)
    // {
    //     for(int j = 0; j <= 16*2;j+=16)
    //     {
    //         x = i;
    //         y = j;
    //         bricks[counter++] = {x, y, 32, 16};
    //         TraceLog(LOG_INFO, TextFormat("%d", counter));
    //     }
    // }

    x = 0;
    y = 16 * 4;
    bricks[counter++] = {x, y, 32, 16};
    
    x = 32;
    bricks[counter++] = {x, y, 32, 16};

    x = 64;
    bricks[counter++] = {x, y, 32, 16};

}
void GameAssetManager::CleanUp()
{
    TraceLog(LOG_INFO, "Cleaning Game Asset Manager");
    delete instance;
}

#pragma endregion