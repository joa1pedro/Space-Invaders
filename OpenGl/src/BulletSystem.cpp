#include "BulletSystem.h"
#include "ResourceManager.h"
#include <Bullet.h>

BulletSystem::BulletSystem(std::shared_ptr<SpriteRenderer> renderer)
	: Renderer(renderer) { }

void BulletSystem::Update(float deltaTime, unsigned int window_width, unsigned int window_height){
	for (Bullet* go : PlayerBullets) {
		if (!go->Destroyed) {
			go->Move(deltaTime, window_width, window_height);
		}
	}
	for (Bullet* go : EnemyBullets) {
		if (!go->Destroyed) {
			go->Move(deltaTime, window_width, window_height);
		}
	}
}

void BulletSystem::Draw(){

	// Drawing Player Bullet
	for (auto it = PlayerBullets.begin(); it != PlayerBullets.end();) {
		if ((*it)->Destroyed) {
			delete* it;
			it = PlayerBullets.erase(it);
		}
		else {
			(*it)->Draw(*Renderer);
			++it;
		}
	}

	// Drawing Enemies Bullets
	for (auto it = EnemyBullets.begin(); it != EnemyBullets.end();) {
		if ((*it)->Destroyed) {
			delete* it;
			it = EnemyBullets.erase(it);
		}
		else {
			(*it)->Draw(*Renderer);
			++it;
		}
	}
}

Bullet* BulletSystem::FireBullet(int id, glm::vec2 pos, glm::vec2 velocity, bool playerBullet){
	Texture2D defaultTex = ResourceManager::GetTexture("laser.png");
	
	// TODO change for scaling
	glm::vec2 size = {
		defaultTex.Width/4,
		defaultTex.Height/2 };
	
	// Default Color
	glm::vec3 color = { 1.0, 1.0, 1.0 };

	// Instantiate the Bullet dinamically
	Bullet* bullet = new Bullet(pos, size, defaultTex, color, velocity, playerBullet);
	
	// Add the bullet to the corresponding vector
	if (playerBullet) 
		PlayerBullets.push_back(bullet);
	else 
		EnemyBullets.push_back(bullet);
	
	return bullet;
}

GameObject* BulletSystem::FireEffect(int id, glm::vec2 pos, glm::vec2 velocity)
{
	return nullptr;
}
