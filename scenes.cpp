#include "scenes.h"
#include "raylib.h"
#include "constants.h"
#include "raymath.h"

#include <math.h>
#include <iostream>

#pragma region SceneManager

SceneManager* SceneManager::instance;
SceneManager* SceneManager::GetInstance()
{
    if(!instance)
    {
        instance = new SceneManager();
    }
    return instance;
}

void SceneManager::AddScene(const char* sceneName, Scene* scene)
{
    scenes[sceneName] = scene;
}

void SceneManager::SetScene(const char* sceneName)
{
    auto it = scenes.find(sceneName);

    if(it != scenes.end())
    {
        if (currentScene != nullptr)
        {
            currentScene->CleanUp();
        }
        currentScene = it->second;
        currentScene->Initialize();
    }
    else
    {
        TraceLog(LOG_ERROR, "Error: Scene does not exist!");
        TraceLog(LOG_ERROR, sceneName);
    }
}

void SceneManager::UpdateScene()
{
    currentScene->Update();
}

void SceneManager::RenderScene()
{
    currentScene->Render();
}

void SceneManager::InputScene()
{
    currentScene->Input();
}

void SceneManager::CleanUpScene()
{
    currentScene->CleanUp();
}

#pragma endregion
#pragma region StartScene

void StartScene::Initialize()
{
    // gameAssetManager.GenerateQuadsPaddles();
    paddles = GameAssetManager::GetInstance()->GetPaddles();

    skin = GameAssetManager::GetInstance()->skin;
    size = GameAssetManager::GetInstance()->size;

    GameAssetManager::GetInstance()->paddle.paddle = paddles[size + (4 * (skin))];

    paddle = GameAssetManager::GetInstance()->paddle;

    paddle.position = {VIRTUAL_WIDTH / 2 - 10, (VIRTUAL_HEIGHT / 2) + 45};
    
    // fontBm = LoadFont("C:/Users/nikhi/Documents/nikhil/projects/cpp/Break Out/assets/fonts/font.ttf");
}

void StartScene::Render()
{
    
    DrawText("Break Out!", (VIRTUAL_WIDTH / 3) - (30), (VIRTUAL_HEIGHT / 3), 40, RAYWHITE);

    
    DrawText("Play", VIRTUAL_WIDTH / 2, (VIRTUAL_HEIGHT / 2) + 20, 20, playColor);

    if(paddleSelected)
    {
        DrawTexturePro(AssetLoader::getInstance()->getBlocks(), paddle.paddle, {paddle.position.x - (paddle.paddle.width/ 6), paddle.position.y  - (paddle.paddle.height/ 6), paddle.paddle.width * (paddle_scale_factor + 0.3f), paddle.paddle.height * (paddle_scale_factor + 0.3f)},{0, 0},0, BLACK);

        DrawText("<", paddle.position.x - (paddle.paddle.width/ 6) - 10,  paddle.position.y  - (paddle.paddle.height/ 6) + 3, 10, WHITE);
        DrawText(">", paddle.position.x + paddle.paddle.width * (paddle_scale_factor + 0.23f), paddle.position.y  - (paddle.paddle.height/ 6) + 3, 10, WHITE);
    }
    DrawTexturePro(AssetLoader::getInstance()->getBlocks(), paddle.paddle, {paddle.position.x, paddle.position.y, paddle.paddle.width * paddle_scale_factor, paddle.paddle.height * paddle_scale_factor},{0, 0},0, WHITE);

    DrawText("Quit", VIRTUAL_WIDTH / 2, (VIRTUAL_HEIGHT / 2) + 60, 20, quitColor);

}

void StartScene::Update()
{
    if(highlighted == 0)
    {
        playColor = BLUE;
        quitColor = WHITE;
        paddleSelected = false;
    }
    else if (highlighted == 1)
    {
        playColor = WHITE;
        quitColor = WHITE;
        paddleSelected = true;
    }
    else
    {
        playColor = WHITE;
        quitColor = BLUE;
        paddleSelected = false;
    }
}

void StartScene::Input()
{
    if(IsKeyPressed(KEY_UP))
    {
        highlighted = (highlighted - 1) % 3;
    }
    else if(IsKeyPressed(KEY_DOWN))
    {
        highlighted = (highlighted + 1) % 3;
    }

    else if(IsKeyPressed(KEY_ENTER))
    {
        if(highlighted == 0)
        {
            SceneManager::GetInstance()->SetScene("play_scene");
        }
    }
    else if(IsKeyPressed(KEY_RIGHT) && highlighted == 1)
    {
        skin = (skin + 1) % 4;
        paddle.paddle = paddles[size + (4 * (skin))];
        GameAssetManager::GetInstance()->skin = skin;
    }
    else if(IsKeyPressed(KEY_LEFT) && highlighted == 1)
    {
        skin -= 1;
        if (skin < 0)
        {
            skin = 3;
        }
        paddle.paddle = paddles[size + (4 * (skin))];
        GameAssetManager::GetInstance()->SetSkin(skin);
    }
}

void StartScene::CleanUp()
{
    TraceLog(LOG_DEBUG, TextFormat("Start Scene Scene cleaned up."));
}

#pragma endregion

#pragma region PlayScene

void PlayScene::Initialize()
{
    paddles = GameAssetManager::GetInstance()->GetPaddles();
    skin = GameAssetManager::GetInstance()->skin;
    size = GameAssetManager::GetInstance()->size;
    paddle.paddle = paddles[size + 4 * (skin)];
    paddle.position = { VIRTUAL_WIDTH / 2 - 32,VIRTUAL_HEIGHT-13};

    ball.ball = GameAssetManager::GetInstance()->balls[0];
    std::cout<<ball.ball.x<<"\t"<<ball.ball.y<<"\t"<<ball.ball.width<<"\t"<<ball.ball.height<<"\t";
    ball.position = {paddle.position.x + 32, paddle.position.y - 10};
}

void PlayScene::Render()
{
    DrawText("Break Out!", VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2, 2, RAYWHITE);
    DrawTexturePro(AssetLoader::getInstance()->getBlocks(), paddle.paddle, {paddle.position.x, paddle.position.y, paddle.paddle.width * paddle_scale_factor, paddle.paddle.height * paddle_scale_factor},{0, 0},0, WHITE);
    DrawTexturePro(AssetLoader::getInstance()->getBlocks(), ball.ball, {ball.position.x, ball.position.y, ball.ball.width, ball.ball.height}, {0, 0}, 0, WHITE);
}

void PlayScene::Update()
{
    if(game_states == 0)
    {
        ball.position.x = paddle.position.x + 32;
    }
    else        // Bounce Ball
    {
        ball.position.x = ball.position.x + ball_dx * GetFrameTime();
        ball.position.y = ball.position.y + ball_dy * GetFrameTime();

        if(ball.position.x < 0)
        {
            ball.position.x = 0;
            ball_dx = -ball_dx;
        }
        if(ball.position.x >= VIRTUAL_WIDTH - 8)
        {
            ball.position.x = VIRTUAL_WIDTH - 8;
            ball_dx = -ball_dx;
        }
        if(ball.position.y <= 0)
        {
            ball.position.y = 0;
            ball_dy = -ball_dy;
        }

    }
    if(BallCollides())
    {
        ball_dy = -ball_dy;
    }
    if(dx < 0)
    {
        paddle.position.x = MAX(0, paddle.position.x + dx * GetFrameTime());
    }
    else
    {
        paddle.position.x = MIN(VIRTUAL_WIDTH - paddle.paddle.width, paddle.position.x + dx * GetFrameTime());
    }
}

bool PlayScene::BallCollides()
{
    if(ball.position.x > paddle.position.x + paddle.paddle.width || paddle.position.x > ball.position.x + ball.ball.width)
    {
        return false;
    }
    if(ball.position.y > paddle.position.y + paddle.paddle.height || paddle.position.y > ball.position.y + ball.ball.height)
    {
        return false;
    }
    return true;
}

void PlayScene::CleanUp()
{
}

void PlayScene::Input()
{
    if(IsKeyDown(KEY_LEFT))
    {
        dx = -PADDLE_SPEED;
    }
    else if(IsKeyDown(KEY_RIGHT))
    {
        dx = PADDLE_SPEED;
    }
    else
    {
        dx = 0;
    }
    if(game_states == 0 && IsKeyPressed(KEY_SPACE))
    {
        game_states = 1;
    }
}

#pragma endregion
