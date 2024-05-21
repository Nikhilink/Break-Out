#pragma once

#include "raylib.h"
#include<unordered_map>
#include <vector>
#include <cmath>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

class Paddle
{
    public:
        Rectangle paddle;
        Rectangle position;

};

class Ball
{
    public:
        Rectangle ball;
        Rectangle position;
};

class Brick
{
    public:
        Rectangle brick;
        Rectangle position;

        int asset_counter = 0;

        int color;
        int tier;

        bool inplay = true;

        Brick(int x, int y)
        {
            position.x = x;
            position.y = y;
            position.width = 32;
            position.height = 16;
        }

        
};

enum GameStates
{
    Ready, Play, Pause, Lose
};

class LevelGenerator
{
    public:
        static std::vector<Brick> CreateMap(int& level)
        {
            std::vector<Brick> bricks;

            int num_rows = GetRandomValue(1, 5);

            int num_cols = GetRandomValue(7, 13);

            num_cols = num_cols % 2 == 0 & (num_cols + 1) | num_cols;

            int highest_tier = MIN(3, std::floor(level / 5));

            int highest_color = MIN(5, level % 5 + 3);

            for(int y = 0; y < num_rows;y++)
            {
                bool skip_pattern = GetRandomValue(1, 2) == 1 ? true : false;

                bool alternate_pattern = GetRandomValue(1, 2) == 1 ? true : false;

                int alternate_color_1 = GetRandomValue(1, highest_color);
                int alternate_color_2 = GetRandomValue(1, highest_color);

                int alternate_tier_1 = GetRandomValue(0, highest_tier);
                int alternate_tier_2 = GetRandomValue(0, highest_tier);

                bool skip_flag = GetRandomValue(1, 2) == 1 ? true : false;

                bool alternate_flag = GetRandomValue(1, 2) == 1 ? true : false;

                int solid_color = GetRandomValue(1, highest_color);
                int solid_tier = GetRandomValue(0, highest_tier);

                for(int x = 0; x < num_cols;x++)
                {
                    if(skip_pattern && skip_flag)
                    {
                        skip_flag = !skip_flag;

                        continue;
                    }
                    else
                    {
                        skip_flag = !skip_flag;
                    }

                    Brick brick(
                            (x - 1) * 32 + 8 + (15 - num_cols) * 16, 
                            y * 16 + 16
                        );
                    if(alternate_pattern && alternate_flag)
                    {
                        brick.color = alternate_color_1;
                        brick.tier = alternate_tier_1;
                        alternate_flag = !alternate_flag;
                    }
                    else
                    {
                        brick.color = alternate_color_2;
                        brick.tier = alternate_tier_2;
                        alternate_flag = !alternate_flag;
                    }

                    if(!alternate_pattern)
                    {
                        brick.color = solid_color;
                        brick.tier = solid_tier;
                    }

                    bricks.push_back(brick);
                }
            }

            if(bricks.size() == 0)
            {
                return CreateMap(level);
            }
            else
            {
                return bricks;
            }
        }
};

#define MAX_PARTICLES 1000
#define GRAVITY 100.0f

class Particle
{
    public:
        Vector2 position;
        Vector2 velocity;
        float life;
        float size;
        Color color;
        float rotation;
        float rotationSpeed;
        bool active;
};

class ParticleSystem
{
    Particle particles[MAX_PARTICLES];

    std::unordered_map<int, Color> colors;

    Vector2 origin;
    float spawnRate;
    float spawnCounter;
    Texture2D texture;

    public:

        ParticleSystem()
        {
            colors[0] = {91, 110, 225, 88};
            colors[1] = {75, 105, 47, 41};
            colors[2] = {172, 50, 50, 67};
            colors[3] = {118, 66, 138, 54};
            colors[4] = {223, 113, 38, 87};
        }
        void InitParticleSystem(Vector2 origin, float spawnRate, Texture2D texture);

        void UpdateParticleSystem(float deltaTime);

        void DrawParticlesSystem();

        void SpawnParticle(Rectangle spawnArea, int color);
};