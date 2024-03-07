#pragma once

//#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Level.h"
#include <vector>
#include "Player.h"

const glm::vec2 PLAYER_SIZE(100.0f, 40.0f);
const glm::vec2 PLAYER_OFFSET(-10.0f, 0.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(300.0f);

// Represents the current state of the SpaceInvaders
enum GameState {
    ACTIVE,
    MENU,
    WIN,
    LOSE
};

class SpaceInvaders
{
public:
    GameState State;
    bool Keys[1024];
    bool Mouse[1024];
    unsigned int Width, Height;

    std::vector<Level> Levels;
    unsigned int CurrentLevel = 0;

    SpaceInvaders(unsigned int width, unsigned int height);

    void Init();

    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render();
    void DoCollisions();

    float spawnDelay = 0.0f;
    bool playerHit = false;
    unsigned int Points;
    unsigned int Lives = 3;
private:
    glm::vec2 playerStartPos;
};
