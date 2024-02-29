#pragma once
#include "GameObject.h"
#include "texture.h"
#include "BulletSystem.h"

class Enemy : public GameObject {
public:
	Enemy(std::shared_ptr<BulletSystem> bs, GameObject go, Texture2D sprite2);

	std::shared_ptr<BulletSystem> bulletSystem;
	Texture2D SecondSprite;

	void SetVelocity(float movementScale, float spriteSwapScale);

	// Move function of the enemy. Return true if the enemy has hit the edge of the screen
	bool Move(float deltaTime, unsigned int window_width, unsigned int window_height);

	// Change sprites of the enemy
	void SwapSprites();

	// Inverts the movement in the X axis
	void InvertMovement();

	//Drops the enemy 1 tile in y axis
	void DropEnemy();

	// Shoots a bullet for the Particle System
	void Shoot();
	bool canShoot = true;
	glm::vec2 bulletVelocity = { 0.0f, 250.0f };

	bool invertedTick = false;
	float updateRateTime = 0.0f;
	glm::vec2 FixedFPSPos;
	bool UseFixedFPS = false;
	float levelUpdateRate = 1.0f;
};