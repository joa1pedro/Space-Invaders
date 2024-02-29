#pragma 
#include <vector>
#include <memory>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Player.h"
#include "Enemy.h"
#include "BulletSystem.h"

class Level {
public:
    std::vector<GameObject> EnemiesGOs;
    std::vector<Enemy> Enemies;
    std::vector<GameObject> BricksGOs;

    Level(const char* file, unsigned int levelWidth, unsigned int levelHeight, std::shared_ptr<BulletSystem> bulletSystem);

    //Update Function
    void Update(float deltaTime, unsigned int window_width, unsigned int window_height);

    // Loads level from file
    void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight, std::shared_ptr<BulletSystem> bulletSystem);

    float elapsedTime = 0.0f;
    // Render level
    void Draw(SpriteRenderer& renderer);
    
    // Check if the level is completed
    bool IsCompleted();

    float bulletTimeCounter = 0.0f;

    float timeBetweenBullets = 3.0f;

    unsigned int Points;
    unsigned int Lives = 3;
private:
    // Scaling factor of the enemy movement when it reaches the corner
    float enemyMovementScaling = 0.10f;
    //Sprite Swap Scaling must be inverse since its the time it takes to change sprites
    float enemySpriteSwapScaling = 0.10f;
};

