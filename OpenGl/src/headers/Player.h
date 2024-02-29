#pragma once
#include "GameObject.h"
#include <map>
#include <vector>
#include <string>
#include <BulletSystem.h>
#include <Bullet.h>

class Player : public GameObject {
public:
	Player(std::shared_ptr<BulletSystem> bulletSystem, Texture2D sprite,
		glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	
	std::shared_ptr<BulletSystem> bulletSystem;
	
	void Shoot();
	bool canShoot = true;
private:
	glm::vec2 bulletVelocity = { 0.0f, -800.0f };
	Bullet* currentBullet = nullptr;
	int currentBulletType = 1;
};

