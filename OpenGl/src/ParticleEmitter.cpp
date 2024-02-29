#include "ParticleEmitter.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

ParticleEmitter::ParticleEmitter(Shader shader, Texture2D texture, unsigned int amount)
    : shader(shader), texture(texture), amount(amount)
{
    this->Init();
}

void ParticleEmitter::Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset)
{
    // Add new particles 
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->FirstUnusedParticle();
        this->RespawnParticle(this->Bullets[unusedParticle], object, offset);
    }

    // Update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& p = this->Bullets[i];

        // Reduce lifetime
        p.Lifetime -= dt; 

        // Particle is alive, update
        if (p.Lifetime > 0.0f){	
            p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}

void ParticleEmitter::Draw()
{
    // Additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.Use();
    for (const Particle &particle : this->Bullets)
    {
        if (particle.Lifetime > 0.0f)
        {
            this->shader.SetVector2f("offset", particle.Position);
            this->shader.SetVector4f("color", particle.Color);
            this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // Reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleEmitter::Init()
{
    // Set up mesh and attribute properties
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);

    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // Create this->amount default particle instances
    for (unsigned int i = 0; i < this->amount; ++i)
        this->Bullets.push_back(Particle());
}

// Stores the index of the last particle used (for quick access to next dead particle)
unsigned int lastUsedParticle = 0;
unsigned int ParticleEmitter::FirstUnusedParticle()
{
    // First search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->Bullets[i].Lifetime <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }

    // Otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->Bullets[i].Lifetime <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }

    // All particles are taken
    // Override the first one 
    // (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleEmitter::RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.Position = object.Position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Lifetime = 1.0f;
    particle.Velocity = object.Velocity * 0.1f;
}