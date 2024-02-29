#include "SpaceInvaders.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "BulletSystem.h"
#include "ParticleEmitter.h"
#include "Bullet.h"
#include "TextRenderer.h"
#include "sstream"
#include <iostream>
#include <memory>

std::shared_ptr<SpriteRenderer> Renderer;
std::shared_ptr<BulletSystem> Bullets;
std::shared_ptr<Player> player;
std::shared_ptr<ParticleEmitter> Particles;
std::shared_ptr<TextRenderer> Text;

SpaceInvaders::SpaceInvaders(unsigned int width, unsigned int height)
    : State(MENU), Keys(), Width(width), Height(height), Points(0) { }


void SpaceInvaders::Init()
{

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


    // Load the fonts for the Text Renderer
    Text = std::make_shared<TextRenderer>(this->Width, this->Height);
    Text->Load("res/fonts/target.otf", 24);

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

void SpaceInvaders::Update(float deltaTime)
{
    if (this->State == ACTIVE) {
        // Update Particles
        //Particles->Update(deltaTime, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));

        //Update Particle Bullets
        Bullets->Update(deltaTime, this->Width, this->Height);

        //Update objects in Level
        this->Levels[this->CurrentLevel].Update(
            deltaTime, this->Width, this->Height);

        //Do Collisions
        if (playerHit) {
            spawnDelay += deltaTime;
            if (spawnDelay >= 1.0f) {
                playerHit = false;
                spawnDelay = 0.0f;
            }
        }
        else {
            this->DoCollisions();
        }
    }
}

void SpaceInvaders::ProcessInput(float deltaTime)
{
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
    if (this->State == MENU)
    {
        if (this->Keys[GLFW_KEY_ENTER])
            this->State = ACTIVE;
    }
}

void SpaceInvaders::Render()
{
    if (this->State == WIN) {
        Text->RenderText("You Win", 250, this->Height/2, 1.0f);
    }

    if (this->State == LOSE) {
        Text->RenderText("You Lose", 250, this->Height/2, 1.0f);
    }

    if (this->State == MENU) {
        Text->RenderText("Press ENTER to start", 250, this->Height/2, 1.0f);
    }

    if (this->State == ACTIVE) {
        if (this->Lives == 0) {
            this->State = LOSE;
        }

        //Draw Level
        if (this->Levels[this->CurrentLevel].Enemies.empty()) {
            this->State = WIN;
        }
        else {
            this->Levels[this->CurrentLevel].Draw(*Renderer);
        }

        // Draw Player
        player->Draw(*Renderer);
        
        // Draw Particles	
        Particles->Draw();

        // Draw Bullets
        Bullets->Draw();

        // Draw Texts
        std::stringstream ss;
        ss << this->Points;
        Text->RenderText("Score:" + ss.str(), 10.0f, 30.0f, 1.0f);
        ss.str("");
        ss << this->Lives;
        Text->RenderText("Lives:" + ss.str(), this->Width-150.0f, 30.0f, 1.0f);
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
    // X Axis Collision only
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // Y Axis Collision only
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // 2 Axis Collision, real Collision
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
                this->Points += 100;
                break; // Exit the loop after destroying one enemy
            }
        }
    }
    for (Bullet* bullet : Bullets->EnemyBullets) {
        if (CheckCollision(*bullet, *player)) {
            bullet->Destroyed = true;
            playerHit = true;
            this->Lives -= 1;
        }
    }
}