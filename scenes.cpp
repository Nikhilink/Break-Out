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

    DrawText("Press Enter to Select", VIRTUAL_WIDTH / 3 + 50, VIRTUAL_HEIGHT - 20, 2, GREEN);
    DrawText("Press P to Pause while Playing the Game", VIRTUAL_WIDTH / 3, VIRTUAL_HEIGHT - 10, 2, GREEN);
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
        AssetLoader::getInstance()->PlayTrack("select");
    }
    else if(IsKeyPressed(KEY_DOWN))
    {
        highlighted = (highlighted + 1) % 3;
        AssetLoader::getInstance()->PlayTrack("select");
    }

    else if(IsKeyPressed(KEY_ENTER))
    {
        if(highlighted == 0)
        {
            AssetLoader::getInstance()->PlayTrack("confirm");
            SceneManager::GetInstance()->SetScene("play_scene");
        }
    }
    else if(IsKeyPressed(KEY_RIGHT) && highlighted == 1)
    {
        skin = (skin + 1) % 4;
        paddle.paddle = paddles[size + (4 * (skin))];
        AssetLoader::getInstance()->PlayTrack("paddle-hit");
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
        AssetLoader::getInstance()->PlayTrack("paddle-hit");
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
    game_states = Ready;
    level = 1;
    health = 5;
    paddles       = GameAssetManager::GetInstance()->GetPaddles();
    skin          = GameAssetManager::GetInstance()->skin;
    size          = GameAssetManager::GetInstance()->size;
    bricks        = LevelGenerator::CreateMap(level);
    brick_asset   = GameAssetManager::GetInstance()->bricks;
    hearts        = GameAssetManager::GetInstance()->hearts;
    totalBricks   = LevelGenerator::totalBricks;

    for(int i = 0;i < bricks.size();i++)
    {
        bricks[i].asset_counter = (bricks[i].color * 4) + bricks[i].tier;
    }
    paddle.paddle = paddles[size + 4 * (skin)];
    paddle.position = { VIRTUAL_WIDTH / 2 - 32,VIRTUAL_HEIGHT-13, paddle.paddle.width  * paddle_scale_factor, paddle.paddle.height * paddle_scale_factor};

    ball.ball = GameAssetManager::GetInstance()->balls[2];
    ball.position = {paddle.position.x + 32, paddle.position.y - 10, ball.ball.width,ball.ball.height};

    Texture2D particle = LoadTexture("C:/Users/nikhi/Documents/nikhil/projects/cpp/Break Out/assets/images/particle.png");
    particleSystem.InitParticleSystem({400,300}, 0.01f, particle);
}

void PlayScene::Render()
{
    DrawTexturePro(AssetLoader::getInstance()->getBlocks(), paddle.paddle, paddle.position,{0, 0},0, WHITE);
    DrawTexturePro(AssetLoader::getInstance()->getBlocks(), ball.ball, {ball.position.x, ball.position.y, ball.ball.width, ball.ball.height}, {0, 0}, 0, WHITE);

    // float x = 0, y = 0;
    // for(int i = 0;i < 21;i++)
    // {
    //     DrawTextureRec(AssetLoader::getInstance()->getBlocks(), brick_asset[i], {x, y}, WHITE);\
    //     x += 32;
    //     y += 16;
    // }
    for(int i = 0;i < bricks.size();i++)
    {
        if(bricks[i].inplay)
        {
            DrawTexturePro(AssetLoader::getInstance()->getBlocks(), brick_asset[bricks[i].asset_counter], bricks[i].position,{0, 0}, 0, WHITE);
        }
    }
    // DrawTexturePro(AssetLoader::getInstance()->getBlocks(), brick_asset[20], {100,100,200,200},{0, 0}, 0, WHITE);
    DrawText(TextFormat("Score: %d", score), VIRTUAL_WIDTH - 150, 0, 2, WHITE);
    DrawText(TextFormat("Level: %d",level), 0, 2, 2, WHITE);
    int life_counter = 0;
    float x = VIRTUAL_WIDTH - 60, y = 2;
    while(life_counter < health)
    {
        DrawTexturePro(AssetLoader::getInstance()->getHearts(), hearts[0], {x, y, 10, 9}, {0, 0}, 0, WHITE);
        x += 12;
        life_counter++;
    }
    while(life_counter < max_health)
    {
        DrawTexturePro(AssetLoader::getInstance()->getHearts(), hearts[1], {x, y, 10, 9}, {0, 0}, 0, WHITE);
        x += 12;
        life_counter++;
    }
    particleSystem.DrawParticlesSystem();
    if(game_states == Lose)
    {
        DrawRectangle((VIRTUAL_WIDTH / 2) - 110, (VIRTUAL_HEIGHT / 2) - 110, 220, 220, {48, 52, 109, 190});
        DrawRectangle((VIRTUAL_WIDTH / 2) - 100, (VIRTUAL_HEIGHT / 2) - 100, 200, 200, {68, 36, 52, 190});
        DrawText("You Lose,\nPress Enter to Return to Menu Screen", (VIRTUAL_WIDTH / 2) - 70, (VIRTUAL_HEIGHT / 2), 2, RED);
    }
    if(game_states == Pause)
    {
        DrawRectangle((VIRTUAL_WIDTH / 2) - 110, (VIRTUAL_HEIGHT / 2) - 110, 220, 220, {48, 52, 109, 190});
        DrawRectangle((VIRTUAL_WIDTH / 2) - 100, (VIRTUAL_HEIGHT / 2) - 100, 200, 200, {68, 36, 52, 190});
        DrawText("Game Paused!\n Press Enter to resume", (VIRTUAL_WIDTH / 2) - 50, VIRTUAL_HEIGHT / 2, 2, RAYWHITE);
    }
    if(game_states == Win)
    {
        DrawRectangle((VIRTUAL_WIDTH / 2) - 110, (VIRTUAL_HEIGHT / 2) - 110, 220, 220, {48, 52, 109, 190});
        DrawRectangle((VIRTUAL_WIDTH / 2) - 100, (VIRTUAL_HEIGHT / 2) - 100, 200, 200, {68, 36, 52, 190});
        DrawText("Level Complete!!1", (VIRTUAL_WIDTH / 2) - 50, VIRTUAL_HEIGHT / 2, 4, GREEN);
    }
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
            AssetLoader::getInstance()->PlayTrack("wall_hit");
            ball.position.x = 0;
            ball_dx = -ball_dx;
        }
        if(ball.position.x >= VIRTUAL_WIDTH - 8)
        {
            AssetLoader::getInstance()->PlayTrack("wall_hit");
            ball.position.x = VIRTUAL_WIDTH - 8;
            ball_dx = -ball_dx;
        }
        if(ball.position.y <= 0)
        {
            AssetLoader::getInstance()->PlayTrack("wall_hit");
            ball.position.y = 0;
            ball_dy = -ball_dy;
        }
        if(ball.position.y > VIRTUAL_HEIGHT)
        {
            health--;
            AssetLoader::getInstance()->PlayTrack("hurt");
            if(health <= 0)
            {
                game_states = Lose;
            }
            else
            {
                game_states = Ready;
                ball_dx = GetRandomValue(100,200);
                ball_dy = GetRandomValue(-100, -200);
            }
            
        }

        for(int i = 0;i < bricks.size();i++)
        {
            if(bricks[i].inplay && CheckCollisionRecs(bricks[i].position, ball.position))
            {
                if(bricks[i].breakable)
                {
                    score = score + (bricks[i].tier * 75, bricks[i].color * 25);
                    if(bricks[i].tier > 0)
                    {

                        bricks[i].tier--;
                        AssetLoader::getInstance()->PlayTrack("brick-hit-1");
                        particleSystem.SpawnParticle(bricks[i].position, bricks[i].color);
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
                        AssetLoader::getInstance()->PlayTrack("brick-hit-1");
                        particleSystem.SpawnParticle(bricks[i].position, bricks[i].color);
                        bricks[i].inplay = false;
                        int total_bricks = 0;
                        for(int j = 0;j < bricks.size();j++)                       // Assuming Order is Maintained and No Elements are being removed
                        {
                            if(!bricks[j].inplay && bricks[j].breakable)                                   // All Bricks are removed
                            {
                                total_bricks++;
                            }
                        }
                        TraceLog(LOG_INFO,TextFormat("%d calc Bricks\t%d Total Bricks", total_bricks, totalBricks));
                        if(total_bricks == totalBricks)                           // Level Complete
                        {
                            if(health <= max_health)
                            {
                                health++;
                            }
                            game_states = Win;
                        }
                        // if(bricks[i].color == 1)
                        // {
                            
                        // }
                        // else
                        // {
                        //     bricks[i].color--;
                        // }
                    }
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
                else if(ball.position.y < bricks[i].position.y)
                {
                    ball_dy = -ball_dy;
                    ball.position.y = bricks[i].position.y - 8;
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
        AssetLoader::getInstance()->PlayTrack("paddle-hit");
        ball_dy = -ball_dy;
        ball.position.y -= 8;
        if(ball.position.x < paddle.position.x + (paddle.position.width / 2) && dx < 0)
        {
            ball_dx = -45 + -(8 * (paddle.position.x + paddle.position.width / 2 - ball.position.x));
        }
        else if(ball.position.x > paddle.position.x + (paddle.position.width / 2) && dx > 0)
        {
            ball_dx = 45 + (8 * fabs(paddle.position.x + paddle.position.width / 2 - ball.position.x));
        }
    }
    if(dx < 0)
    {
        paddle.position.x = MAX(0, paddle.position.x + dx * GetFrameTime() * 1.10);
    }
    else
    {
        paddle.position.x = MIN(VIRTUAL_WIDTH - paddle.position.width, paddle.position.x + dx * GetFrameTime() * 1.10);
    }

    particleSystem.UpdateParticleSystem(GetFrameTime());
}

void PlayScene::LoadNextLevel()
{
    bricks = LevelGenerator::CreateMap(++level);
    totalBricks = LevelGenerator::totalBricks;
    for(int i = 0;i < bricks.size();i++)
    {
        bricks[i].asset_counter = (bricks[i].color * 4) + bricks[i].tier;
    }
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
        AssetLoader::getInstance()->PlayTrack("recover");
        game_states = Play;
    }
    if(game_states == Play && IsKeyPressed(KEY_P))
    {
        AssetLoader::getInstance()->PlayTrack("pause");
        AssetLoader::getInstance()->PauseGameMusic();
        game_states = Pause;
    }
    if(game_states == Play && IsKeyPressed(KEY_H))
    {
        health = max_health;
    }
    if(game_states == Pause && IsKeyPressed(KEY_ENTER))
    {
        AssetLoader::getInstance()->PlayTrack("recover");
        AssetLoader::getInstance()->ResumeGameMusic();
        game_states = Play;
    }
    if(game_states == Lose && IsKeyPressed(KEY_ENTER))
    {
        AssetLoader::getInstance()->PlayTrack("hurt");
        SceneManager::GetInstance()->SetScene("start_scene");
    }
    if(game_states == Win && IsKeyPressed(KEY_ENTER))
    {
        AssetLoader::getInstance()->PlayTrack("victory");
        LoadNextLevel();
        game_states = Ready;
    }
}

#pragma endregion
