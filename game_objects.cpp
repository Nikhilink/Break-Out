#include "game_objects.h"

int LevelGenerator::totalBricks;

void ParticleSystem::InitParticleSystem(Vector2 origin, float spawnRate, Texture2D texture)
{
    this->origin = origin;
    this->spawnRate = spawnRate;
    this->spawnCounter = 0;
    this->texture = texture;

    for(int i = 0; i < MAX_PARTICLES;i++)
    {
        this->particles[i].active = false;
    }
}

void ParticleSystem::UpdateParticleSystem(float deltaTime)
{
    this->spawnCounter += deltaTime;
    

    while(this->spawnCounter >= this->spawnRate)
    {
        // SpawnParticle();
        this->spawnCounter -= this->spawnCounter;
    }

    for(int i = 0;i < MAX_PARTICLES;i++)
    {
        if(this->particles[i].active)
        {
            this->particles[i].velocity.y += GRAVITY * deltaTime;

            this->particles[i].position.x += this->particles[i].velocity.x * deltaTime;
            this->particles[i].position.y += this->particles[i].velocity.y * deltaTime;
            this->particles[i].rotation += this->particles[i].rotationSpeed * deltaTime;

            this->particles[i].life -= deltaTime;

            if(this->particles[i].life <= 0)
            {
                this->particles[i].active = false;
            }
            else
            {
                float alpha = this->particles[i].life / (this->particles[i].life + deltaTime);
                this->particles->color.a = (unsigned char)(alpha * 255);
            }
        }
    }
}

void ParticleSystem::DrawParticlesSystem()
{

    for(int i = 0;i < MAX_PARTICLES;i++)
    {
        if(this->particles[i].active)
        {
            Vector2 origin = {(float)this->texture.width / 2, (float)this->texture.height / 2};
            DrawTexturePro(this->texture, {0, 0, (float)this->texture.width, (float)this->texture.height},
            {this->particles[i].position.x, this->particles[i].position.y, this->particles[i].size, this->particles[i].size},
            {0,0}, this->particles[i].rotation, this->particles[i].color);
        }
    }
}

void ParticleSystem::SpawnParticle(Rectangle spawnArea, int color)
{
    int spawned = 0;
    for(int i = 0; i < MAX_PARTICLES&& spawned < 10;i++)
    {
        if(!this->particles[i].active)
        {
            this->particles[i].position = {
                (float)GetRandomValue(spawnArea.x, spawnArea.x + spawnArea.width),
                (float)GetRandomValue(spawnArea.y, spawnArea.y + spawnArea.height)
            };
            this->particles[i].velocity = {(float)GetRandomValue(-100,100) / 100.0f, (float)GetRandomValue(-100,100) / 100.0f};
            this->particles[i].life = (float)GetRandomValue(50, 100) / 100.0f;
            this->particles[i].size = (float)GetRandomValue(5, 15);
            this->particles[i].color = colors[color];
            this->particles[i].rotation = GetRandomValue(0,360);
            this->particles[i].rotationSpeed = (float)GetRandomValue(-50, 50) / 10.0f;
            this->particles[i].active = true;

            spawned++;
        }
    }
}