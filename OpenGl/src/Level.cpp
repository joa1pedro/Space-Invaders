#include "Level.h"
#include <sstream>
#include <fstream>
#include <string>
#include <random>

const char* LEVEL_PATH = "res/levels/level.txt";

Level::Level(const char* file, unsigned int levelWidth, unsigned int levelHeight, std::shared_ptr<BulletSystem> bulletSystem){
    Load(file, levelWidth, levelHeight, bulletSystem);
}

void Level::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight, std::shared_ptr<BulletSystem> bulletSystem){
    // Clear old data
    this->EnemiesGOs.clear();
    this->Enemies.clear();
    this->BricksGOs.clear();

    // Load GameObjects from file
    std::vector<GameObject> levelObjects = ResourceManager::LoadGameObjectsFromFile(LEVEL_PATH);
    for (GameObject go : levelObjects) {
        if (go.IsSolid) {
            this->BricksGOs.push_back(go);
        }
        else {
            Enemies.emplace_back( Enemy{ bulletSystem, go,
                ResourceManager::GetTexture(
                    go.SpritePath.insert(go.SpritePath.find_last_of('.'), "2")
                )});
        }
    }
}

// Function to get a random index from a vector
int getRandomIndex(int vecSize) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, vecSize - 1);
    return dis(gen);
}

void Level::Update(float deltaTime, unsigned int window_width, unsigned int window_height) {
    // Erase the destroyed enemies
    Enemies.erase(std::remove_if(Enemies.begin(), Enemies.end(), [](const Enemy& enemy) {
        return enemy.Destroyed;
    }), Enemies.end());

    bulletTimeCounter += deltaTime;
    if (bulletTimeCounter >= timeBetweenBullets) {
        // Time to shoot a Enemy bullet
        this->Enemies[getRandomIndex(static_cast<int>(this->Enemies.size()))].Shoot();
        bulletTimeCounter = 0.0f;
    }

    // Move the enemies
    bool invert = false;
    for (Enemy& enemy : this->Enemies) {
        if(enemy.Move(deltaTime, window_width, window_height)){
            // Some enemy has hit the corner. 
            // Setting the flag
            invert = true;
        }        
    }

    // Some enemy has hit the corner previously
    if (invert) {
        for (Enemy& enemy : this->Enemies) {
            // [1] Invert every enemy movement in the X axis
            enemy.InvertMovement();
            // [2] Drop them down 1 tile
            enemy.DropEnemy();
            // [3] Increase the velocity of them all
            enemy.SetVelocity(enemyMovementScaling, enemySpriteSwapScaling);
        } 
    }
}

void Level::Draw(SpriteRenderer& renderer){
    for (Enemy& enemy : this->Enemies) {
        if (!enemy.Destroyed)
            enemy.Draw(renderer);
    }

    for (GameObject& go : this->BricksGOs) {
        if (!go.Destroyed)
            go.Draw(renderer);
    }
}

bool Level::IsCompleted(){
    if (Enemies.empty()) {
        return true;
    }
    return false;
}
