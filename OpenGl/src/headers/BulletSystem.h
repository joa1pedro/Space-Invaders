#pragma once
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

class Bullet;
class GameObject;
class SpriteRenderer;

class BulletSystem
{
public:
	BulletSystem(std::shared_ptr<SpriteRenderer> renderer);
	void Update(float deltaTime, unsigned int window_width, unsigned int window_height);
	void Draw();
	Bullet* FireBullet(int id, glm::vec2 pos, glm::vec2 velocity, bool playerBullet);
	GameObject* FireEffect(int id, glm::vec2 pos, glm::vec2 velocity);
	std::vector<Bullet*> EnemyBullets;
	std::vector<Bullet*> PlayerBullets;
private:
	std::shared_ptr<SpriteRenderer> Renderer;
};

