#include "Player.h"
#include <iostream>
#include "Bullet.h"

Player::Player(std::shared_ptr<BulletSystem> particleSystem, Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity)
    : particleSystem(particleSystem), GameObject(pos, size, sprite, color, velocity) { };

void Player::Shoot() {
    if (canShoot) {
        glm::vec2 bulletP = { this->Position.x + this->Size.x / 2 , this->Position.y };

        currentBullet = particleSystem->FireBullet(currentBulletType, 
            bulletP, bulletVelocity, true);
        canShoot = false;
    }
    if (currentBullet->Destroyed) {
        canShoot = true;
    }
}