#pragma once
#include "GameObject.h"
#include <map>
#include <vector>
#include <string>
#include <BulletSystem.h>
#include <irrKlang.h>
#include <Bullet.h>

using namespace irrklang;


class Player : public GameObject {
public:
	Player(ISoundEngine* soundEngine, std::shared_ptr<BulletSystem> bulletSystem, Texture2D sprite,
		glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	
	std::shared_ptr<BulletSystem> bulletSystem;
	ISoundEngine* SoundEngine;
	
	void Shoot();
	void Reset(glm::vec2 playerStartPos);
	bool canShoot = true;
	bool playerHit = false;
private:
	glm::vec2 bulletVelocity = { 0.0f, -400.0f };
	Bullet* currentBullet = nullptr;
	int currentBulletType = 1;
};

