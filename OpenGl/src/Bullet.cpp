#include "Bullet.h"

Bullet::Bullet(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, bool isPlayerBullet)
	: GameObject(pos, size, sprite, color, velocity), InitialPosition(pos), isPlayerBullet(isPlayerBullet){ }

glm::vec2 Bullet::Move(float deltaTime, unsigned int window_width, unsigned int window_height)
{
	this->Position += this->Velocity * deltaTime;

	if (this->Position.y + this->Size.y >= window_height || this->Position.y <= 0.0f) {
		// Bullet reached the edge of the screen
		this->Destroyed = true;
	}

	return this->Position;
}