
#include "raylib.h"

#include "constants.h"
#include "logging.h"

#include "scenes.h"
#include "asset_loader.h"
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))


int main(void)
{
    // -----------------------------------------------------------------------------------------------------------------
    SetTraceLogCallback(log);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    // -----------------------------------------------------------------------------------------------------------------

    // -----------------------------------------------------------------------------------------------------------------
    InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Break Out!");
    InitAudioDevice();
    SetWindowMinSize(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    // -----------------------------------------------------------------------------------------------------------------

    // -----------------------------------------------------------------------------------------------------------------
    AssetLoader* loader = AssetLoader::getInstance();

    StartScene startScene;
    PlayScene playScene;
    SceneManager* manager = SceneManager::GetInstance();

    manager->AddScene("start_scene", &startScene);
    manager->AddScene("play_scene", &playScene);
    manager->SetScene("start_scene");

    Texture background = loader->getBackground();
    // -----------------------------------------------------------------------------------------------------------------

    SetTargetFPS(60); 

    RenderTexture2D target = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    AssetLoader::getInstance()->PlayGameMusic();
    while (!WindowShouldClose())    
    {
        float scale = MIN((float)GetScreenWidth() / VIRTUAL_WIDTH, (float)GetScreenHeight() / VIRTUAL_HEIGHT);
        AssetLoader::getInstance()->UpdateGameMusic();
        manager->InputScene();
        manager->UpdateScene();

        BeginTextureMode(target);

            ClearBackground(BLACK);
            DrawTextureEx(background, {0, 0}, 0,2.0f, WHITE);
            manager->RenderScene();
            // DrawFPS(0,0);

        EndTextureMode();

        BeginDrawing();

            ClearBackground(BLACK);
            DrawTexturePro(
                target.texture, 
                {0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height},
                {
                    (GetScreenWidth() - ((float)VIRTUAL_WIDTH * scale)) * 0.5f, 
                    (GetScreenHeight() - ((float)VIRTUAL_HEIGHT * scale)) * 0.5f, 
                    (float)VIRTUAL_WIDTH * scale, 
                    (float)VIRTUAL_HEIGHT * scale,
                },
                {0, 0},
                0.0f, WHITE
                );
        EndDrawing();
        
    }
    GameAssetManager::CleanUp();
    AssetLoader::getInstance()->StopGameMusic();
    AssetLoader::destroyInstance();
    CloseWindow();
    return 0;
}

