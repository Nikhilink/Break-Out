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

    paddle.position = {VIRTUAL_WIDTH / 2 - 10, (VIRTUAL_HEIGHT / 2) + 45, paddle.paddle.width, paddle.paddle.height};
    
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
    paddles       = GameAssetManager::GetInstance()->GetPaddles();
    skin          = GameAssetManager::GetInstance()->skin;
    size          = GameAssetManager::GetInstance()->size;
    bricks        = LevelGenerator::CreateMap(level);
    brick_asset   = GameAssetManager::GetInstance()->bricks;

    for(int i = 0;i < bricks.size();i++)
    {
        bricks[i].asset_counter = (bricks[i].color * 4) + bricks[i].tier;
    }
    paddle.paddle = paddles[size + 4 * (skin)];
    paddle.position = { VIRTUAL_WIDTH / 2 - 32,VIRTUAL_HEIGHT-13, paddle.paddle.width  * paddle_scale_factor, paddle.paddle.height * paddle_scale_factor};

    ball.ball = GameAssetManager::GetInstance()->balls[2];
    ball.position = {paddle.position.x + 32, paddle.position.y - 10, ball.ball.width,ball.ball.height};

    
}

void PlayScene::Render()
{
    DrawTexturePro(AssetLoader::getInstance()->getBlocks(), paddle.paddle, paddle.position,{0, 0},0, WHITE);
    DrawTexturePro(AssetLoader::getInstance()->getBlocks(), ball.ball, {ball.position.x, ball.position.y, ball.ball.width, ball.ball.height}, {0, 0}, 0, WHITE);
    if(game_states == Lose)
    {
        DrawText("You Lose,\n Press Enter to play again", VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2, 2, RAYWHITE);
    }
    if(game_states == Pause)
    {
        DrawText("Game Paused!", VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2, 2, RAYWHITE);
    }

    for(int i = 0;i < bricks.size();i++)
    {
        if(bricks[i].inplay)
        {
            DrawTexturePro(AssetLoader::getInstance()->getBlocks(), brick_asset[bricks[i].asset_counter], bricks[i].position,{0, 0}, 0, WHITE);
        }
    }
    DrawText(TextFormat("Score: %d", score), VIRTUAL_WIDTH - 100, 0, 2, WHITE);
}

void PlayScene::Update()
{
    if(game_states == Ready)
    {
        ball.position.x = paddle.position.x + (paddle.position.width / 2);
        ball.position.y = paddle.position.y - (ball.position.height + 2);
    }
    else if(game_states == Play)     // Bounce Ball
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
        if(ball.position.y > VIRTUAL_HEIGHT)
        {
            game_states = Lose;
        }

        for(int i = 0;i < bricks.size();i++)
        {
            if(bricks[i].inplay && CheckCollisionRecs(bricks[i].position, ball.position))
            {
                
                score = score + (bricks[i].tier * 200, bricks[i].color * 25);

                if(bricks[i].tier > 0)
                {
                    bricks[i].tier--;
                    bricks[i].asset_counter = (bricks[i].color * 4) + bricks[i].tier;
                    // if(bricks[i].color == 1)
                    // {
                    //     bricks[i].tier--;
                    //     bricks[i].color = 5;
                    //     bricks[i].asset_counter = (bricks[i].color * 4) + bricks[i].tier;
                    // }
                    // else
                    // {
                    //     bricks[i].color--;
                    //     bricks[i].asset_counter = (bricks[i].color * 4) + bricks[i].tier;
                    // }
                }
                else
                {
                    bricks[i].inplay = false;
                    // if(bricks[i].color == 1)
                    // {
                        
                    // }
                    // else
                    // {
                    //     bricks[i].color--;
                    // }
                }
                if(ball.position.x + 2 < bricks[i].position.x && ball_dx > 0)
                {
                    ball_dx = -ball_dx;
                    ball.position.x = bricks[i].position.x - 8;
                }
                else if(ball.position.x + 6 > bricks[i].position.x + bricks[i].position.width && ball_dx < 0)
                {
                    ball_dx = -ball_dx;
                    ball.position.x = bricks[i].position.x + 32;
                }
                else
                {
                    ball_dy = -ball_dy;
                    ball.position.y = bricks[i].position.y + 16;
                }
                // ball_dy *= 1.02f;
                // if(bricks[i].color == 1)
                // {
                //     TraceLog(LOG_INFO, TextFormat("Deleting Brick %d\t%d", bricks[i].color, bricks[i].tier));
                //     bricks[i].inplay = false;
                // }
                // else
                // {
                //     TraceLog(LOG_INFO, TextFormat("%d", bricks[i].color));

                //     bricks[i].color--;
                // }
            }
        }

    }
    if(CheckCollisionRecs(paddle.position,ball.position))
    {
        ball_dy = -ball_dy;
        ball.position.y -= 8;
        if(ball.position.x < paddle.position.x + (paddle.position.width / 2) && dx < 0)
        {
            ball_dx = -50 + -(8 * (paddle.position.x + paddle.position.width / 2 - ball.position.x));
        }
        else if(ball.position.x > paddle.position.x + (paddle.position.width / 2) && dx > 0)
        {
            ball_dx = 50 + (8 * fabs(paddle.position.x + paddle.position.width / 2 - ball.position.x));
        }
    }
    if(dx < 0)
    {
        paddle.position.x = MAX(0, paddle.position.x + dx * GetFrameTime());
    }
    else
    {
        paddle.position.x = MIN(VIRTUAL_WIDTH - paddle.position.width, paddle.position.x + dx * GetFrameTime());
    }
}

bool PlayScene::BallCollides()
{
    if(ball.position.x > paddle.position.x + paddle.position.width || paddle.position.x > ball.position.x + ball.position.width)
    {
        return false;
    }
    if(ball.position.y > paddle.position.y + paddle.position.height || paddle.position.y > ball.position.y + ball.position.height)
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
    if(IsKeyDown(KEY_LEFT) && (game_states == Play || game_states == Ready))
    {
        dx = -PADDLE_SPEED;
    }
    else if(IsKeyDown(KEY_RIGHT) && (game_states == Play || game_states == Ready))
    {
        dx = PADDLE_SPEED;
    }
    else
    {
        dx = 0;
    }
    if(game_states == Ready && IsKeyPressed(KEY_SPACE))
    {
        game_states = Play;
    }
    if(game_states == Play && IsKeyPressed(KEY_P))
    {
        game_states = Pause;
    }
    if(game_states == Pause && IsKeyPressed(KEY_ENTER))
    {
        game_states = Play;
    }
    if(game_states == Lose && IsKeyPressed(KEY_ENTER))
    {
        game_states = Ready;
    }
}

#pragma endregion
