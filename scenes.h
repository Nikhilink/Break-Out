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
    int skin;
    int size;
    Paddle paddle;
    float paddle_scale_factor = 0.7f;
    const float PADDLE_SPEED = 140;
    float dx = 0;

    Ball ball;
    public:
        void Initialize() override;

        void Input() override;

        void Update() override;

        void Render() override;

        void CleanUp() override;
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