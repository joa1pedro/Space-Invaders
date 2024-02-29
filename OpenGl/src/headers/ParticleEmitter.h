#pragma once
#include <vector>
#include "GameObject.h"
#include "shader.h"
#include "texture.h"

struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float Lifetime;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Lifetime(0.0f) { }
};

class ParticleEmitter{
public:
    ParticleEmitter(Shader shader, Texture2D texture, unsigned int amount);
    
    // Update all particles
    void Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    
    // Render all particles
    void Draw();
private:
    std::vector<Particle> Bullets;
    unsigned int amount;

    Shader shader;
    Texture2D texture;
    unsigned int VAO;

    // Initializes buffer and vertex attributes
    void Init();
    
    // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int FirstUnusedParticle();

    void RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};