#include "SpaceInvaders.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "BulletSystem.h"
#include "ParticleEmitter.h"
#include "Bullet.h"
#include "Obstacle.h"
#include "TextRenderer.h"
#include "sstream"
#include <iostream>
#include <memory>
#include <irrKlang.h>

std::shared_ptr<SpriteRenderer> Renderer;
std::shared_ptr<BulletSystem> Bullets;
std::shared_ptr<Player> player;
std::shared_ptr<ParticleEmitter> Particles;
std::shared_ptr<TextRenderer> Text;
std::shared_ptr<Obstacle> obstacles[4];
ISoundEngine* SoundEngine = createIrrKlangDevice();

using namespace irrklang;

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

    //Initialize Sound Engine
    SoundEngine->play2D("res/audio/breakout.mp3", true);

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
        
    // Start Player
    playerStartPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, // Middle of the Screen
        this->Height + PLAYER_OFFSET.x - PLAYER_SIZE.y);
    player = std::make_shared<Player>(SoundEngine, Bullets, ResourceManager::GetTexture("player.png"), playerStartPos, PLAYER_SIZE);

    for (int i = 0; i < 4; i++) {
        obstacles[i] = std::make_shared<Obstacle>(glm::vec2{ 60+(200*i), 400 });
    }
}

void SpaceInvaders::Update(float deltaTime)
{
    if (this->State == ACTIVE) {
        // Update Particles
        //Particles->Update(deltaTime, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));

        //Update Bullets
        Bullets->Update(deltaTime, this->Width, this->Height);

        //Update objects in Level
        this->Levels[this->CurrentLevel].Update(
            deltaTime, this->Width, this->Height);

        //Do Collisions
        if (playerHit) {
            player->Reset(playerStartPos);
            spawnDelay += deltaTime;
            if (spawnDelay >= 1.0f) {
                playerHit = false;
                spawnDelay = 0.0f;
                player->playerHit = false;
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
        if (this->Keys[GLFW_KEY_SPACE] || this->Mouse[GLFW_MOUSE_BUTTON_LEFT])
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

        // Draw Obstacles
        for (auto obstacle : obstacles) {
            obstacle->Draw(*Renderer);
        }

        // Draw Texts
        std::stringstream ss;
        ss << this->Points;
        Text->RenderText("Score:" + ss.str(), 10.0f, 30.0f, 1.0f);
        ss.str("");
        ss << this->Lives;
        Text->RenderText("Lives:" + ss.str(), this->Width-150.0f, 30.0f, 1.0f);
        ss.str("");

        //Debug mode
        ss << Bullets->PlayerBullets.size();
        Text->RenderText("Player Bullets:" + ss.str(), 10.0f, 60.0f, 1.0f);
        ss.str("");
        ss << Bullets->EnemyBullets.size();
        Text->RenderText("Enemy Bullets:" + ss.str(), 10.0f, 90.0f, 1.0f);
    }
}

// Collisions between Bullets and the Player
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

// Collisions between Bullets and Enemies
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

// Collisions between Bullets and Obstacles Blocks
bool CheckCollision(Bullet& one, GameObject& two) // AABB - AABB collision
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
    // Check Collisions for the player bullets
    if (Bullets && !Bullets->PlayerBullets.empty()) {
        // Obstacles collision agains player bullets
        for (auto &obstacle : obstacles) {
            for (auto &block : obstacle->blocks)
                if (!block.Destroyed && CheckCollision(*Bullets->PlayerBullets[0], block)) {
                    // Remove the player bullet from the stack
                    Bullets->PlayerBullets.pop_back();

                    //Reset player shooting cooldown
                    player->canShoot = true;

                    //Set the obstacle to be destroyed
                    block.Destroyed = true;

                    // Exit the loop after destroying one obstacle
                    return;
                }
        }

        // Enemy collision against player bullets
        for (Enemy& enemy : this->Levels[this->CurrentLevel].Enemies) {
            if (!enemy.Destroyed && CheckCollision(*Bullets->PlayerBullets[0], enemy)) {
                // A Bullet has hit an enemy

                // Remove the player bullet from the stack
                Bullets->PlayerBullets.pop_back();
                
                //Reset player shooting cooldown
                player->canShoot = true;

                // Set the enemy to Destroyed, it is going to be removed from the stack on the next draw call
                enemy.Destroyed = true; 

                //Player the audio for destroying an enemy
                SoundEngine->play2D("res/audio/invaderkilled.wav", false);

                // Give player the points for that enemy
                this->Points += enemy.PointsForDestruction;

                // Exit the loop after destroying one enemy
                break; 
            }
        }
    }

    // Check collisions for the Enemy bullets
    for (Bullet* bullet : Bullets->EnemyBullets) {

        // Collision against player 
        if (CheckCollision(*bullet, *player)) {
            // A bullet has collided with the player

            // Set the bullet to destroyed, it is going to be removed from the stck on the next draw call
            bullet->Destroyed = true;

            // Set a cooldown for the player Hit
            playerHit = true;

            // Decrease player lifepoints
            this->Lives -= 1;
        }
        
        //Collision against obstacles
        for (auto &obstacle : obstacles) {
            for(auto &block : obstacle->blocks)
            if (!block.Destroyed && CheckCollision(*bullet, block)) {
                // Set the bullet to destroyed, it is going to be removed from the stck on the next draw call
                bullet->Destroyed = true;

                //Set the obstacle to be destroyed
                block.Destroyed = true;

                // Exit the loop after destroying one obstacle
                break;
            }
        }
    }
}