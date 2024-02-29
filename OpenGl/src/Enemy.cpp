#include "Enemy.h"
#include <iostream>

Enemy::Enemy(std::shared_ptr<BulletSystem> ps, GameObject go, Texture2D secondSprite)
	: GameObject(go.Position, go.Size, go.Sprite, go.Color, go.Velocity),
	  SecondSprite(secondSprite), FixedFPSPos(go.Position), particleSystem(ps)
{ }

bool Enemy::Move(float deltaTime, unsigned int window_width, unsigned int window_height){
	bool spriteChanged = false;
	updateRateTime += deltaTime;

	if (updateRateTime >= levelUpdateRate) {
		SwapSprites();
		spriteChanged = true;

		if(UseFixedFPS)
			this->Position = FixedFPSPos;
	}
	if (spriteChanged) {
		updateRateTime = 0.0f;
		spriteChanged = false;
	}
	
	FixedFPSPos += this->Velocity * deltaTime;
	
	if(!UseFixedFPS)
		this->Position += this->Velocity * deltaTime;

	if (UseFixedFPS) {
		if (FixedFPSPos.x <= 0.0f || FixedFPSPos.x + this->Size.x >= window_width){
			// Invader reached edge
			return true;
		}

		if (FixedFPSPos.y + this->Size.y >= window_height){
			// Invader reached bottom
		}
	}
	else {
		if (this->Position.x <= 0.0f || this->Position.x + this->Size.x >= window_width){
			// Invader reached edge
			return true;
		}

		if (this->Position.y + this->Size.y >= window_height){
			// Invader reached bottom
		}
	}

	return false;
}

void Enemy::DropEnemy() {
	// Drop down by the height of the invader
	if (UseFixedFPS)
		FixedFPSPos.y += this->Size.y;
	else
		this->Position.y += this->Size.y;
}

void Enemy::InvertMovement() {
	this->Velocity.x = -this->Velocity.x;
}

void Enemy::SetVelocity(float movementScale, float spriteSwapScale){
	this->Velocity = glm::vec2{ this->Velocity.x + movementScale, this->Velocity.y };
	this->levelUpdateRate += spriteSwapScale;
}

void Enemy::SwapSprites() {
	Texture2D tempSprite;
	tempSprite = this->Sprite;
	this->Sprite = SecondSprite;
	SecondSprite = tempSprite;
}

void Enemy::Shoot() {
	if (canShoot) {
		glm::vec2 bulletP = { this->Position.x + this->Size.x / 2 , this->Position.y };
		particleSystem->FireBullet(1,
			bulletP, bulletVelocity, false);
		canShoot = false;
	}
}