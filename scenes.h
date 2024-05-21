#pragma once

#include "raylib.h"

#include "asset_loader.h"

#include <unordered_map>
#include <string>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

class Scene
{
    public:
        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual void CleanUp() = 0;
        virtual void Input() = 0;
};
class PlayScene : public Scene
{
    std::unordered_map<int, Rectangle> paddles;
    std::vector<std::vector<Vector2>> map;
    std::unordered_map<int, Rectangle> brick_asset;
    std::vector<Brick> bricks;
    std::unordered_map<int, Rectangle> hearts;
    
    int skin;
    int size;
    Paddle paddle;
    float paddle_scale_factor = 0.7f;
    const float PADDLE_SPEED = 140;
    float dx = 0;

    float ball_dx = GetRandomValue(100,200);
    float ball_dy = GetRandomValue(-100, -200);

    Ball ball;

    int score = 0;

    GameStates game_states = Ready;        // 0 Start 1 Play 2 Paused

    int level = 21;
    int health = 3;
    int max_health = 5;

    ParticleSystem particleSystem;
    public:
        void Initialize() override;

        void Input() override;

        void Update() override;

        void Render() override;

        void CleanUp() override;
        bool BallCollides();

};
class StartScene : public Scene
{
    int highlighted = 0;
    Color playColor = WHITE;
    Color quitColor = WHITE;
    
    bool paddleSelected = false;
    Font fontBm;
    std::unordered_map<int, Rectangle> paddles;
    float paddle_scale_factor = 0.7f;

    int skin;
    int size;
    Paddle paddle;
    
    public:
        
        void Initialize() override;

        void Update() override;

        void Render() override;

        void CleanUp() override;

        void Input() override;

        // My Methods
};

class SceneManager
{
    static SceneManager* instance;
    std::unordered_map<std::string, Scene*> scenes;
    Scene* currentScene = nullptr;

    SceneManager(){}
    public:

        static SceneManager* GetInstance();
        void AddScene(const char* sceneName, Scene* scene);

        void SetScene(const char* sceneName);
        void UpdateScene();

        void RenderScene();
        void InputScene();

        void CleanUpScene();
};