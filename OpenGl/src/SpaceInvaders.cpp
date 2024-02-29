#include "SpaceInvaders.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "BulletSystem.h"
#include "ParticleEmitter.h"
#include "Bullet.h"
#include <iostream>
#include <memory>

std::shared_ptr<SpriteRenderer> Renderer;
std::shared_ptr<BulletSystem> Bullets;
std::shared_ptr<Player> player;
std::shared_ptr<ParticleEmitter> Particles;

SpaceInvaders::SpaceInvaders(unsigned int width, unsigned int height)
    : State(ACTIVE), Keys(), Width(width), Height(height) { }


void SpaceInvaders::Init(){

    // Make the Particle Emitter
	ResourceManager::LoadShader(
        "res/shaders/particleVertex.shader", 
        "res/shaders/particleFragment.shader", 
        nullptr, 
             "particle");
	ResourceManager::LoadTexture("textures/particle.png", true, "particle");
	Particles = std::make_shared<ParticleEmitter>(
		ResourceManager::GetShader("particle"),
		ResourceManager::GetTexture("particle"),
		500
	);


    // Load Shaders
    ResourceManager::LoadShader(
        "res/shaders/vertex.shader",
        "res/shaders/fragment.shader",
        nullptr, 
             "sprite");
    
    // Configure Shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    
    // Set render specific controls
    Renderer = std::make_shared<SpriteRenderer>(ResourceManager::GetShader("sprite"));
    Bullets = std::make_shared<BulletSystem>(Renderer);

    // Load textures
    ResourceManager::LoadResources("res/textures");

    this->Levels.emplace_back(Level("res/levels/levelLayout.txt", this->Width, this->Height / 2, Bullets));

    // Configure game objects
    glm::vec2 playerPos = 
        glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, // Middle of the Screen
        this->Height + PLAYER_OFFSET.x - PLAYER_SIZE.y);
        
    player = std::make_shared<Player>(Bullets, ResourceManager::GetTexture("player.png"), playerPos, PLAYER_SIZE);
}

void SpaceInvaders::Update(float deltaTime){
    // Update Particles
    //Particles->Update(deltaTime, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));

    //Update Particle Bullets
    Bullets->Update(deltaTime, this->Width, this->Height);

    //Update objects in Level
    this->Levels[this->CurrentLevel].Update(
        deltaTime, this->Width, this->Height);

    //Do Collisions
    this->DoCollisions();
}

void SpaceInvaders::ProcessInput(float deltaTime){
    if (this->State == ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * deltaTime;
        // Move the Player
        if (this->Keys[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT])
        {
            if (player->Position.x >= 0.0f)
                player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT])
        {
            if (player->Position.x <= this->Width - player->Size.x)
                player->Position.x += velocity;
        }
        if (this->Keys[GLFW_KEY_SPACE])
            player->Shoot();
    }
}

void SpaceInvaders::Render(){
    if (this->State == ACTIVE) {
        //Draw Level
        this->Levels[this->CurrentLevel].Draw(*Renderer);

        // Draw player
        player->Draw(*Renderer);
        // draw particles	
        Particles->Draw();
        Bullets->Draw();
    }
}

bool CheckCollision(Bullet& one, Player& two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

bool CheckCollision(Bullet& one, Enemy& two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

void SpaceInvaders::DoCollisions()
{
    if (Bullets && !Bullets->PlayerBullets.empty()) {
        for (Enemy& enemy : this->Levels[this->CurrentLevel].Enemies) {
            if (!enemy.Destroyed && CheckCollision(*Bullets->PlayerBullets[0], enemy)) {
                Bullets->PlayerBullets.pop_back();
                player->canShoot = true;
                enemy.Destroyed = true;
                break; // Exit the loop after destroying one enemy
            }
        }

        for (Bullet* bullet : Bullets->EnemyBullets) {
            if (CheckCollision(*bullet, *player)) {
                std::cout << "Player Hit" << std::endl;
            }
        }
    }
}