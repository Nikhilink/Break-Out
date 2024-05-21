#include "game_objects.h"

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
            this->particles[i].life -= deltaTime;

            if(this->particles[i].life <= 0)
            {
                this->particles[i].active = false;
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
            DrawTexture(this->texture, this->particles[i].position.x - this->texture.width / 2, this->texture.height / 2, this->particles[i].color);
        }
    }
}

void ParticleSystem::SpawnParticle(Vector2 position)
{
    for(int i = 0; i < MAX_PARTICLES;i++)
    {
        if(!this->particles[i].active)
        {
            this->particles[i].position = position;
            this->particles[i].velocity = {GetRandomValue(-100,100) / 100.0f, GetRandomValue(-100,100) / 100.0f};
            this->particles[i].life = 200;// GetRandomValue(50,100) / 100.0f;
            this->particles[i].size = GetRandomValue(5, 15);
            this->particles[i].color = {
                (unsigned char)GetRandomValue(0, 255), 
                (unsigned char)GetRandomValue(0, 255), 
                (unsigned char)GetRandomValue(0, 255), 
                180};
            // this->particles[i].color = RED;
            this->particles[i].active = true;

            break;
        }
    }
}