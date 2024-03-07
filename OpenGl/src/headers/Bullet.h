#pragma once
#include "GameObject.h"

class Bullet : public GameObject {
public:
	Bullet(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, bool isPlayerBullet);
	glm::vec2 InitialPosition;
	bool isPlayerBullet;
	glm::vec2 Move(float deltaTime, unsigned int window_width, unsigned int window_height);
};